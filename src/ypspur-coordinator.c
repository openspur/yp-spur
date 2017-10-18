/* high level I/O */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <strings.h>

/* low level I/O */
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <setjmp.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* ボディパラメータ */
#include <shvel-param.h>

/* yp-spur 用 */
#include <ypspur-coordinator.h>
#include <serial.h>
#include <param.h>
#include <control.h>
#include <command.h>
#include <odometry.h>
#include <ypprotocol.h>
#include <adinput.h>
#include <ssm_spur_handler.h>
#include <utility.h>
#include <yprintf.h>

/* ライブラリ用 */
#include <ypspur.h>
#include <cartesian2d.h>

#include <pthread.h>


void escape_road( void );
void emergency( int );
#if HAVE_SIGLONGJMP
sigjmp_buf ctrlc_capture;
#elif HAVE_LONGJMP
jmp_buf ctrlc_capture;
#endif
int g_emergency;


#if defined(__MINGW32__)
BOOL WINAPI win32_ctrlc_handler( DWORD type )
{
	fprintf( stderr, "\n" );
	g_emergency = 1;
#ifdef HAVE_SSM
	/* SSM終了処理 */
	if( !option( OPTION_WITHOUT_SSM ) )
		end_ypspurSSM(  );
#endif
	if( !( option( OPTION_WITHOUT_DEVICE ) ) )
	{
		serial_close(  );
	}
	
	return TRUE;
}
#else
void emergency( int sig )
{
	fprintf( stderr, "\n" );
	g_emergency = 1;
#if HAVE_SIGLONGJMP
	siglongjmp( ctrlc_capture, 1 );
#elif HAVE_LONGJMP
	longjmp( ctrlc_capture, 1 );
#else
#ifdef HAVE_SSM
	/* SSM終了処理 */
	if( !option( OPTION_WITHOUT_SSM ) )
		end_ypspurSSM(  );
#endif
	if( !( option( OPTION_WITHOUT_DEVICE ) ) )
	{
		serial_close(  );
	}

	exit( 0 );
#endif
}
#endif

void escape_road( void )
{
#if defined(__MINGW32__)
	if( !SetConsoleCtrlHandler( win32_ctrlc_handler, TRUE ) )
	{
		yprintf( OUTPUT_LV_ERROR, "Error: Win32 Ctrl+C handler registration failed.\n" );
	}
#else
	signal( SIGINT, emergency );
#endif
}

/* main */
int main( int argc, char *argv[] )
{
	pthread_t command_thread;
	pthread_t control_thread;
	pthread_t update_thread;
	int command_thread_en;
	int control_thread_en;
	int update_thread_en;
	Ver_t version;
	Param_t driver_param;
	int i, ret;
	ParametersPtr param;
	char paramfile[512];
	int quit;

	hook_pre_global();

	ret = arg_analyze( argc, argv );
	if( option( OPTION_DAEMON ) )
	{
#if HAVE_FORK
		pid_t pid;

		pid = fork(  );
		if( pid < 0 )
		{
			return -1;
		}
		else if( pid != 0 )
		{
			return 0;
		}
		setsid(  );
		if( chdir( "/" ) < 0 )
		{
			yprintf( OUTPUT_LV_ERROR, "Failed to chdir.\n" );
			return EXIT_FAILURE;
		}
		close( STDIN_FILENO );
		close( STDOUT_FILENO );
		close( STDERR_FILENO );
#else
		yprintf( OUTPUT_LV_ERROR, "Daemon mode is not supported in your system.\n" );
		return EXIT_FAILURE;
#endif
	}
	if( option( OPTION_SHOW_HELP ) )
	{
		arg_help( argc, argv );
		return EXIT_SUCCESS;
	}
	if( option( OPTION_SHOW_LONGHELP ) )
	{
		arg_longhelp( argc, argv );
		return EXIT_SUCCESS;
	}
	if( option( OPTION_SHOW_PARAMHELP ) )
	{
		param_help(  );
		return EXIT_SUCCESS;
	}
	if( option( OPTION_VERSION ) )
	{
		fprintf( stderr, "YamabicoProject-Spur\n" );
		fprintf( stderr, " Ver. %s\n", PROJECT_VERSION );
		return EXIT_SUCCESS;
	}
	if( !ret )									/* オプション解析に失敗したとき */
		return EXIT_FAILURE;

	yprintf( OUTPUT_LV_PROCESS, "++++++++++++++++++++++++++++++++++++++++++++++++++\n" );
	yprintf( OUTPUT_LV_PROCESS, "YamabicoProject-Spur\n" );
	yprintf( OUTPUT_LV_PROCESS, " Ver. %s\n", PROJECT_VERSION );
	yprintf( OUTPUT_LV_PROCESS, "++++++++++++++++++++++++++++++++++++++++++++++++++\n" );

	/* Ctrl-C割り込みハンドラーの登録 */
	escape_road(  );
	g_emergency = 0;

	/* パラメータを読み込み、セットする */
	param = get_param_ptr(  );

#ifdef HAVE_SSM
	/* SSM初期化 */
	if( !option( OPTION_WITHOUT_SSM ) )
		init_ypspurSSM( param->ssm_id );
#endif

	/* 座標系の初期化、コマンド処理系の初期化 */
	init_coordinate_systems(  );
	init_odometry(  );
	init_spur_command(  );

	fflush( stderr );

	command_thread_en = 0;
	command_thread_en = 0;
	do
	{
		FILE *temp_paramfile = NULL;
		quit = 0;

		yprintf( OUTPUT_LV_PROCESS, "Device Information\n" );
		yprintf( OUTPUT_LV_PROCESS, " Port    : %s \n", param->device_name );

		if( !( option( OPTION_WITHOUT_DEVICE ) ) )
		{
			if( !serial_connect( param->device_name ) )
			{
				// quit=0;でbreakしたら異常終了と判断される
				break;
			}
			if( !( option( OPTION_DO_NOT_USE_YP ) ) )
			{
				int current, age;
				int device_current, device_age;
				sscanf( YP_PROTOCOL_NAME, "YPP:%d:%d", &current, &age );

				yprintf( OUTPUT_LV_PROCESS, " Checking device information...\r" );
				for ( i = 0; i < 3; i++ )
				{
					// Check protocol version
					if( get_version( &version ) == -1 )
					{
						continue;
					}
					if( strstr( version.protocol, "YPP:" ) != version.protocol )
					{
						continue;
					}
					sscanf( version.protocol, "YPP:%d:%d", &device_current, &device_age );
					param->device_version = device_current;
					param->device_version_age = device_age;
					if( device_current - device_age > current || 
							device_current < current - age )
					{
						continue;
					}
					break;
				}
				yprintf( OUTPUT_LV_PARAM, " Vender  : %s\033[K\n", version.vender );
				yprintf( OUTPUT_LV_PARAM, " Product : %s\n", version.product );
				yprintf( OUTPUT_LV_PARAM, " Firmware: %s\n", version.firmware );
				yprintf( OUTPUT_LV_PARAM, " Protcol : %s\n", version.protocol );
				yprintf( OUTPUT_LV_PARAM, " Serialno: %s\n", version.serialno );
				yprintf( OUTPUT_LV_PARAM, "++++++++++++++++++++++++++++++++++++++++++++++++++\n" );
				if( i == 3 )
				{
					yprintf( OUTPUT_LV_ERROR, "Error: Device doesn't have available YP protocol version.\n(Device: %s, coordinator: %s)\n",
						  version.protocol, YP_PROTOCOL_NAME );
					if( option( OPTION_RECONNECT ) && g_emergency == 0 )
					{
						yp_usleep( 500000 );
						continue;
					}
					break;						// quit=0でbreakしたら異常終了と判断
				}
				if( device_current != current )
				{
					if( device_current < current )
					{
						yprintf( OUTPUT_LV_WARNING, "Recommendation: Device protocol version is not latest.\n" );
						yprintf( OUTPUT_LV_WARNING, "Recommendation: Firmware update is recommended.\n" );
					}
					else
					{
						yprintf( OUTPUT_LV_WARNING, "Recommendation: ypspur-coordinator protocol version is not latest.\n" );
						yprintf( OUTPUT_LV_WARNING, "Recommendation: Software update is recommended.\n" );
					}
					yprintf( OUTPUT_LV_WARNING, "++++++++++++++++++++++++++++++++++++++++++++++++++\n" );
				}
			}
			fflush( stderr );

			if( get_parameter( &driver_param ) == -1 )
			{
				continue;
			}
			yprintf( OUTPUT_LV_PARAM, "Driver depending parameters\n" );
			yprintf( OUTPUT_LV_PARAM, " Name          : %s\n", driver_param.robot_name );
			yprintf( OUTPUT_LV_PARAM, " PWM resolution: %s\n", driver_param.pwm_resolution );
			yprintf( OUTPUT_LV_PARAM, " Motor number  : %s\n", driver_param.motor_num );
			yprintf( OUTPUT_LV_PARAM, "++++++++++++++++++++++++++++++++++++++++++++++++++\n" );

			if( strlen( driver_param.pwm_resolution ) <= 0 || 
			    strlen( driver_param.motor_num ) <= 0 )
			{
				yprintf( OUTPUT_LV_ERROR, "Error: Failed to load driver parameters.\n" );
				if( option( OPTION_RECONNECT ) && g_emergency == 0 )
				{
					yp_usleep( 500000 );
					continue;
				}
				break;
			}
		}
		if( !( option( OPTION_PARAM_FILE ) ) )
		{
			// パラメータファイルが指定されておらず、ドライバにパラメータが内蔵されている場合
			if( strcmp( driver_param.robot_name, "embedded" ) == 0 )
			{
				char param[2048];

				yprintf( OUTPUT_LV_MODULE, "Reading device embedded parameter.\n" );
				temp_paramfile = tmpfile(  );
				if( !temp_paramfile )
				{
					yprintf( OUTPUT_LV_ERROR, "Error: Failed to create temporary file.\n" );
					return 0;
				}
				if( !get_embedded_param( param ) )
				{
					yprintf( OUTPUT_LV_ERROR, "Error: Failed to read embedded parameters.\n" );
					if( option( OPTION_RECONNECT ) && g_emergency == 0 )
					{
						yp_usleep( 500000 );
						continue;
					}
					break;
				}
				fprintf( temp_paramfile, "%s", param );
				fseek( temp_paramfile, 0L, SEEK_SET );
			}
			// パラメータファイルが指定されておらず、ドライバにロボット名が登録されている場合
			else if( strlen( driver_param.robot_name ) > 0 && strcmp( driver_param.robot_name, "unknown" ) != 0 )
			{
				strcpy( param->parameter_filename, driver_param.robot_name );
				strcat( param->parameter_filename, ".param" );
			}
		}
		if( temp_paramfile )
		{
			yprintf( OUTPUT_LV_PARAM, "Embedded parameter file\n" );
			if( !set_paramptr( temp_paramfile ) )
			{
				yprintf( OUTPUT_LV_ERROR, "Error: Cannot use embedded parameter.\n" );
				return 0;
			}
		}
		else
		{
			yprintf( OUTPUT_LV_PARAM, "Parameter file: %s\n", param->parameter_filename );
			if( !set_param( param->parameter_filename, paramfile ) )
			{
				yprintf( OUTPUT_LV_ERROR, "Error: Cannot find parameter file.\n" );
				return 0;
			}
		}
		{
			int i;
			for ( i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i++ )
			{
				*pp( YP_PARAM_PWM_MAX, i ) = atoi( driver_param.pwm_resolution );
			}

		}
		yprintf( OUTPUT_LV_PARAM, "++++++++++++++++++++++++++++++++++++++++++++++++++\n\n" );

		if( !( option( OPTION_WITHOUT_DEVICE ) ) )
		{
			// ボーレートの設定
			if( param->speed )
			{
				yprintf( OUTPUT_LV_MODULE, "Setting baudrate to %d baud.\n", param->speed );
			}
			else {
				// 指定されてない場合デフォルトの値
				param->speed = DEFAULT_BAUDRATE;
			}

			ret = set_baudrate( param->speed );
			if( ret == 0 )
			{
				// 設定失敗
				yprintf( OUTPUT_LV_WARNING, "Error: Failed to change baudrate.\n" );

				serial_close(  );

				quit = 0;
				break;						// quit=0でbreakしたら異常終了と判断
			}
			if (ret == 4)
			{
				// ボーレートの設定未対応
				yprintf( OUTPUT_LV_WARNING, "Info: Baudrate setting is not supported on this device.\n" );
			}
			else 
			{
				// 設定成功
				// 正常ならば何もしない
			}

			if( param->admask )
			{
				yprintf( OUTPUT_LV_MODULE, "Setting admask to %x.\n", param->admask );
				set_admask( param->admask );
			}

			if( option( OPTION_ENABLE_GET_DIGITAL_IO ) )
			{
				yprintf( OUTPUT_LV_MODULE, "Enabling digital io input.\n" );
				set_diomask( 1 );
			}

			if( !( option( OPTION_PARAM_CONTROL ) ) )
			{
				apply_robot_params(  );
			}

			/* サーボをかける */
			SpurUserParamsPtr spur;
			spur = get_spur_user_param_ptr();
			for( i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i++ )
			{
				spur->wheel_mode[i] = MOTOR_CONTROL_VEL;
				spur->wheel_mode_prev[i] = -1;
			}
		}

		yprintf( OUTPUT_LV_MODULE, "YP-Spur coordinator started.\n" );

		/* スレッド初期化 */
		init_command_thread( &command_thread );
		command_thread_en = 1;

		if( !( option( OPTION_WITHOUT_DEVICE ) ) )
		{
			init_control_thread( &control_thread );
			control_thread_en = 1;
		}
		else
		{
			control_thread_en = 0;
		}
		if( option( OPTION_UPDATE_PARAM ) )
		{
			init_param_update_thread( &update_thread, paramfile );
			update_thread_en = 1;
		}
		else
		{
			update_thread_en = 0;
		}

		// オドメトリ受信ループ
#if HAVE_SIGLONGJMP
		if( sigsetjmp( ctrlc_capture, 1 ) != 0 )
		{
			quit = 1;
		}
		else
#elif HAVE_LONGJMP
		if( setjmp( ctrlc_capture ) != 0 )
		{
			quit = 1;
		}
		else
#endif
		{
			if( !( option( OPTION_WITHOUT_DEVICE ) ) )
			{
				odometry_receive_loop(  );
			}
			else
			{
				while( 1 )
					yp_usleep( 1000000 );
			}
			yprintf( OUTPUT_LV_MODULE, "Connection to %s was closed.\n", param->device_name );
		}

		/* 終了処理 */
		if( !( option( OPTION_WITHOUT_DEVICE ) ) )
		{
			serial_close(  );
		}

		if( update_thread_en )
		{
			pthread_cancel( update_thread );
			pthread_join( update_thread, NULL );
			update_thread_en = 0;
		}
		if( control_thread_en )
		{
			pthread_cancel( control_thread );
			pthread_join( control_thread, NULL );
			control_thread_en = 0;
		}
		if( command_thread_en )
		{
			pthread_cancel( command_thread );
			pthread_join( command_thread, NULL );
			command_thread_en = 0;
		}

		if( option( OPTION_RECONNECT ) && quit == 0 )
		{
			init_spur_command(  );
			yp_usleep( 500000 );
			if( !( option( OPTION_WITHOUT_DEVICE ) ) )
			{
				while( !serial_tryconnect( param->device_name ) )
				{
					yp_usleep( 200000 );
				}
			}
			yprintf( OUTPUT_LV_MODULE, "++++++++++++++++++++++++++++++++++++++++++++++++++\n" );
			yp_usleep( 500000 );
			continue;
		}
		break;
	}
	while( 1 );

#ifdef HAVE_SSM
	/* SSM終了処理 */
	if( !option( OPTION_WITHOUT_SSM ) )
		end_ypspurSSM(  );
#endif

	yp_usleep( 200000 );
	fflush( stderr );

	return ( quit ? EXIT_SUCCESS : EXIT_FAILURE );
}

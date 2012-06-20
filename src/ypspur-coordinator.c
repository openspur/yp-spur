/* high level I/O */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <strings.h>
#include <pthread.h>

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

void escape_road( void );
static void emergency( int );
#if HAVE_SIGLONGJMP
sigjmp_buf ctrlc_capture;
#elif HAVE_LONGJMP
jmp_buf ctrlc_capture;
#endif
int g_emergency;

static void emergency( int sig )
{
	fprintf( stderr, "\n" );
	g_emergency = 1;
#if HAVE_SIGLONGJMP
	siglongjmp( ctrlc_capture, 1 );
#elif HAVE_LONGJMP
	longjmp( ctrlc_capture, 1 );
#else
	serial_close(  );
#ifdef HAVE_SSM
	/* SSM終了処理 */
	if( !option( OPTION_WITHOUT_SSM ) )
		end_ypspurSSM(  );
#endif
	exit( 0 );
#endif
}

void escape_road( void )
{
	signal( SIGINT, emergency );
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
	int i, ret;
	ParametersPtr param;
	char paramfile[ 512 ];
	int quit;

	ret = arg_analyze( argc, argv );
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
		param_help( );
		return EXIT_SUCCESS;
	}
	if( option( OPTION_VERSION ) )
	{
		fprintf( stderr, "YamabicoProject-Spur\n" );
		fprintf( stderr, " Ver. %s\n", PACKAGE_VERSION );
		return EXIT_SUCCESS;
	}
	if( !ret )									/* オプション解析に失敗したとき */
		return EXIT_FAILURE;

	yprintf( OUTPUT_LV_PROCESS, "\n" );
	yprintf( OUTPUT_LV_PROCESS, "++++++++++++++++++++++++++++++++++++++++++++++++++\n" );
	yprintf( OUTPUT_LV_PROCESS, "YamabicoProject-Spur\n" );
	yprintf( OUTPUT_LV_PROCESS, " Ver. %s\n", PACKAGE_VERSION );
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

	yprintf( OUTPUT_LV_PARAM, "Parameter file: %s\n", param->parameter_filename );
	if( !set_param( param->parameter_filename, paramfile ) )
	{
		yprintf( OUTPUT_LV_ERROR, "Error: Cannot find parameter file.\n" );
		return 0;
	}
	yprintf( OUTPUT_LV_PARAM, "++++++++++++++++++++++++++++++++++++++++++++++++++\n\n" );

	fflush( stderr );

	command_thread_en = 0;
	command_thread_en = 0;
	do
	{
		quit = 0;

		yprintf( OUTPUT_LV_PROCESS, "Device Infomation\n" );
		yprintf( OUTPUT_LV_PROCESS, " Port    : %s \n", param->device_name );

		if( !( option( OPTION_WITHOUT_DEVICE ) ) )
		{
			if( !serial_connect( param->device_name ) )
			{
				// fprintf( stderr, "Error: init_serial()\n" );
				// quit=0;でbreakしたら異常終了と判断される
				break;
			}
			if( !( option( OPTION_DO_NOT_USE_YP ) ) )
			{
				int current, age;
				sscanf( YP_PROTOCOL_NAME, "YPP:%d:%d", &current, &age );

				yprintf( OUTPUT_LV_PROCESS, " Checking device infomation...\r" );
				for ( i = 0; i < 3; i++ )
				{
					int device_current, device_age;
					// プロトコルがYPであることを確認
					if( get_version( &version ) == -1 )
					{
						continue;
					}
					if( strstr( version.protocol, "YPP:" ) != version.protocol )
					{
						continue;
					}
					sscanf( version.protocol, "YPP:%d:%d", &device_current, &device_age );
					if( device_current - device_age > current || device_current < current )
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

				yprintf( OUTPUT_LV_PROCESS, "++++++++++++++++++++++++++++++++++++++++++++++++++\n" );
				if( i == 3 )
				{
					yprintf( OUTPUT_LV_ERROR, "Error: Device doesn't have available YP protocol version.\n" );
					if( option( OPTION_RECONNECT ) && g_emergency == 0 )
					{
						yp_usleep( 500000 );
						continue;
					}
					break; // quit=0でbreakしたら異常終了と判断
				}
			}
			fflush( stderr );

			if( param->speed )
			{
				yprintf( OUTPUT_LV_MODULE, "Setting baudrate to %d baud.\n", param->speed );
				if( !set_baudrate( param->speed ) )
				{
					yprintf( OUTPUT_LV_WARNING, "Error: Failed to change baudrate.\n" );
					
					serial_close(  );

					quit = 0;
					break;	// quit=0でbreakしたら異常終了と判断
				}
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
			if( state( YP_STATE_MOTOR ) && state( YP_STATE_VELOCITY ) )
			{
				motor_servo(  );
			}
		}

		yprintf( OUTPUT_LV_MODULE, "YP-Spur coordinator started.\n" );

		/* スレッド初期化 */
		init_command_thread( &command_thread );
		pthread_detach( command_thread );
		command_thread_en = 1;

		if( !( option( OPTION_WITHOUT_DEVICE ) ) )
		{
			init_control_thread( &control_thread );
			pthread_detach( control_thread );
			control_thread_en = 1;
		}
		if( option( OPTION_UPDATE_PARAM ) )
		{
			init_param_update_thread( &update_thread, paramfile );
			pthread_detach( update_thread );
			update_thread_en = 1;
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
				while( 1 ) yp_usleep( 1000000 );
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


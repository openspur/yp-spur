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
#include <signal.h>
#include <setjmp.h>
#include <utility.h>

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
	Ver_t version;
	int i, ret;
	ParametersPtr param;
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

	if( output_lv(  ) >= OUTPUT_LV_PROCESS )
	{
		fprintf( stderr, "\n" );
		fprintf( stderr, "++++++++++++++++++++++++++++++++++++++++++++++++++\n" );
		fprintf( stderr, "YamabicoProject-Spur\n" );
		fprintf( stderr, " Ver. %s\n", PACKAGE_VERSION );
		fprintf( stderr, "++++++++++++++++++++++++++++++++++++++++++++++++++\n" );
		fflush( stderr );
	}

	/* Ctrl-C割り込みハンドラーの登録 */
	escape_road(  );
	g_emergency = 0;

#ifdef HAVE_SSM
	/* SSM初期化 */
	if( !option( OPTION_WITHOUT_SSM ) )
		init_ypspurSSM(  );
#endif

	/* 座標系の初期化、コマンド処理系の初期化 */
	init_coordinate_systems(  );
	init_odometry(  );
	init_spur_command(  );

	/* パラメータを読み込み、セットする */
	param = get_param_ptr(  );

	if( output_lv(  ) >= OUTPUT_LV_PARAM )
		fprintf( stderr, "Parameter file: %s\n", param->parameter_filename );
	if( !set_param( param->parameter_filename ) )
	{
		if( output_lv(  ) >= OUTPUT_LV_ERROR )
			fprintf( stderr, "Error: Cannot find parameter file.\n" );
		return 0;
	}
	if( output_lv(  ) >= OUTPUT_LV_PARAM )
		fprintf( stderr, "++++++++++++++++++++++++++++++++++++++++++++++++++\n\n" );

	fflush( stderr );

	do
	{
		quit = 0;

		if( output_lv(  ) >= OUTPUT_LV_PROCESS )
			fprintf( stderr, "Device Infomation\n" );
		if( output_lv(  ) >= OUTPUT_LV_PROCESS )
			fprintf( stderr, " Port    : %s \n", param->device_name );

		if( !( option( OPTION_WITHOUT_DEVICE ) ) )
		{
			if( !serial_connect( param->device_name ) )
			{
				// fprintf( stderr, "Error: init_serial()\n" );
				// quit=0;でbreakしたら異常終了と判断される
				break;
			}
		}

		if( !( option( OPTION_DO_NOT_USE_YP ) ) )
		{
			int current, age;
			sscanf( YP_PROTOCOL_NAME, "YPP:%d:%d", &current, &age );

			if( output_lv(  ) >= OUTPUT_LV_PROCESS )
			{
				fprintf( stderr, " Checking device infomation...\r" );
				fflush( stderr );
			}
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
			if( output_lv(  ) >= OUTPUT_LV_PARAM )
			{
				fprintf( stderr, " Vender  : %s\033[K\n", version.vender );
				fprintf( stderr, " Product : %s\n", version.product );
				fprintf( stderr, " Firmware: %s\n", version.firmware );
				fprintf( stderr, " Protcol : %s\n", version.protocol );
				fprintf( stderr, " Serialno: %s\n", version.serialno );
			}
			if( output_lv(  ) >= OUTPUT_LV_PROCESS )
				fprintf( stderr, "++++++++++++++++++++++++++++++++++++++++++++++++++\n" );
			if( i == 3 )
			{
				if( output_lv(  ) >= OUTPUT_LV_ERROR )
					fprintf( stderr, "Error: Device doesn't have available YP protocol version.\n" );
				if( option( OPTION_RECONNECT ) && g_emergency == 0 )
				{
					yp_usleep( 500000 );
					continue;
				}
				//return EXIT_FAILURE;
				break; // quit=0;でbreakしたら異常終了と判断される
			}
		}
		fflush( stderr );

		if( param->speed && !( option( OPTION_WITHOUT_DEVICE ) ) )
		{
			if( output_lv(  ) >= OUTPUT_LV_MODULE )
				fprintf( stderr, "Setting baudrate to %d baud.\n", param->speed );
			if( !set_baudrate( param->speed ) )
			{
				if( output_lv(  ) >= OUTPUT_LV_WARNING )
					fprintf( stderr, "Error: Failed to change baudrate.\n" );
					
				serial_close(  );
				pthread_cancel( control_thread );
				pthread_cancel( command_thread );
				pthread_join( control_thread, NULL );
				pthread_join( command_thread, NULL );

				quit = 0;
				break;	// quit=0;でbreakしたら異常終了と判断される
			}
		}

		if( param->admask && !( option( OPTION_WITHOUT_DEVICE ) ) )
		{
			if( output_lv(  ) >= OUTPUT_LV_MODULE )
				fprintf( stderr, "Setting admask to %x.\n", param->admask );
			set_admask( param->admask );
		}

		if( option( OPTION_ENABLE_GET_DIGITAL_IO ) && !( option( OPTION_WITHOUT_DEVICE ) ) )
		{
			if( output_lv(  ) >= OUTPUT_LV_MODULE )
				fprintf( stderr, "Enabling digital io input.\n" );
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

		if( output_lv(  ) >= OUTPUT_LV_MODULE )
		{
			fprintf( stderr, "YP-Spur coordinator started.\n" );
			fflush( stderr );
		}
		/* スレッド初期化 */
		init_command_thread( &command_thread );
		pthread_detach( command_thread );
		if( !( option( OPTION_WITHOUT_DEVICE ) ) )
		{
			init_control_thread( &control_thread );
			pthread_detach( control_thread );
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
			if( output_lv(  ) >= OUTPUT_LV_MODULE )
				fprintf( stderr, "Connection to %s was closed.\n", param->device_name );
		}

		/* 終了処理 */
		serial_close(  );

		if( !( option( OPTION_WITHOUT_DEVICE ) ) )
		{
			pthread_cancel( control_thread );
			pthread_join( control_thread, NULL );
		}
		pthread_cancel( command_thread );
		pthread_join( command_thread, NULL );

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
			if( output_lv(  ) >= OUTPUT_LV_MODULE )
				fprintf( stderr, "++++++++++++++++++++++++++++++++++++++++++++++++++\n" );
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

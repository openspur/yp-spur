
/* high level I/O */
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <strings.h>
#include <pthread.h>

/* low level I/O */
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* ボディパラメータ */
#include <shvel-param.h>

/* yp-spur用 */
#include <serial.h>
#include <param.h>
#include <control.h>
#include <command.h>
#include <ipcommunication.h>

/* ライブラリ用 */
#include <ypspur.h>
#include <cartesian2d.h>

SpurUserParams g_spur;

SpurUserParamsPtr get_spur_user_param_ptr(  )
{
	return &g_spur;
}

void command_loop_cleanup( void *data )
{
	int *msq_id;

	msq_id = data;
	if( output_lv(  ) >= OUTPUT_LV_MODULE )
	{
		fprintf( stderr, "Command analyser stopped.\n" );
		fflush( stderr );
	}
	// メッセージキュー削除
	msgctl( *msq_id, IPC_RMID, NULL );
}

void init_spur_command( void )
{
	g_spur.x = 0;
	g_spur.y = 0;
	g_spur.theta = 0;
	g_spur.v = 0;
	g_spur.w = 0;
	g_spur.radius = 0;
	g_spur.tilt = 0;
	g_spur.dir = 0;
	g_spur.run_mode = RUN_STOP;
	g_spur.before_run_mode = -10000;
	g_spur.freeze = 0;
	g_spur.before_freeze = 0;
	pthread_mutex_init( &g_spur.mutex, NULL );
}

/* コマンド受信 */
void command( void )
{
	YPSpur_msg msg, res_msg;
	int msq_id, len;
	char param_name[YP_PARAM_NUM][30] = YP_PARAM_NAME;

	/* initialize message queue */
	msq_id = msgget( get_param_ptr(  )->msq_key, 0666 | IPC_CREAT );
	if( msq_id < 0 )
	{
		/* queue_error */
		if( output_lv(  ) >= OUTPUT_LV_ERROR )
		{
			fprintf( stderr, "Error: Can't initialize message queue.\n" );
			fflush( stderr );
		}
		return;
	}

	// メッセージキューを空にする
	while( 1 )
	{
		if( msgrcv( msq_id, &msg, YPSPUR_MSG_SIZE, YPSPUR_MSG_CMD, IPC_NOWAIT ) == -1 )
		{
			break;
		}
	}

	if( output_lv(  ) >= OUTPUT_LV_MODULE )
	{
		fprintf( stderr, "Command analyser started.\n" );
		fflush( stderr );
	}
	pthread_cleanup_push( command_loop_cleanup, &msq_id );

	while( 1 )
	{
		/* 1コマンド取得 */
		len = msgrcv( msq_id, &msg, YPSPUR_MSG_SIZE, YPSPUR_MSG_CMD, 0 );
		if( len < YPSPUR_MSG_SIZE )
		{
			fprintf( stderr, "ERROR: Invalid command received\n" );
			continue;
		}
		//fprintf( stderr, "one command received\n" );

		pthread_mutex_lock( &g_spur.mutex );
		/* コマンド解析・返信 */
		switch ( msg.type )
		{										/* 関数 */

	  /*--------command_set.c--------*/
		case YPSPUR_SET_POS:
			set_pos_com( msg.cs, msg.data, &g_spur );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: set pos %f %f %f\n", msg.data[0], msg.data[1], msg.data[2] );
			break;
		case YPSPUR_SET_GL_GL:
			set_GL_on_GL_com( msg.data, &g_spur );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: GL on GL  %f %f %f\n", msg.data[0], msg.data[1], msg.data[2] );
			break;
		case YPSPUR_ADJUST:
			set_adjust_com( msg.cs, msg.data, &g_spur );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: adjust  %f %f %f\n", msg.data[0], msg.data[1], msg.data[2] );
			break;
		case YPSPUR_SET_VEL:
			set_vel_com( msg.data, &g_spur );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: vel %f\n", g_spur.v );
			break;
		case YPSPUR_SET_ANGVEL:
			set_ang_vel_com( msg.data, &g_spur );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: w %f\n", g_spur.w );
			break;
		case YPSPUR_SET_ACCEL:
			set_accel_com( msg.data, &g_spur );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: dvel %f\n", g_spur.dv );
			break;
		case YPSPUR_SET_ANGACCEL:
			set_ang_accel_com( msg.data, &g_spur );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: dw %f\n", g_spur.dw );
			break;
		case YPSPUR_SET_TILT:
			set_tilt_com( msg.cs, msg.data, &g_spur );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: tilt %f %f\n", g_spur.dir, g_spur.tilt );
			break;

	  /*--------command_run.c----------*/
		case YPSPUR_LINE:
			line_com( msg.cs, msg.data, &g_spur );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: line %f %f %f\n", g_spur.x, g_spur.y, g_spur.theta );
			break;
		case YPSPUR_STOP_LINE:
			stop_line_com( msg.cs, msg.data, &g_spur );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: stop_line %f %f %f\n", g_spur.x, g_spur.y, g_spur.theta );
			break;
		case YPSPUR_CIRCLE:
			circle_com( msg.cs, msg.data, &g_spur );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: circle %f %f %f\n", g_spur.x, g_spur.y, g_spur.radius );
			break;
		case YPSPUR_SPIN:
			spin_com( msg.cs, msg.data, &g_spur );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: spin %f\n", g_spur.theta );
			break;
		case YPSPUR_ORIENT:
			orient_com( msg.cs, msg.data, &g_spur );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: orient %f\n", g_spur.theta );
			break;
		case YPSPUR_STOP:
			stop_com( msg.data, &g_spur );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: stop\n" );
			break;
		case YPSPUR_FREE:
			free_com( msg.data, &g_spur );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: free\n" );
			// motor_free();
			break;
		case YPSPUR_FREEZE:
			g_spur.freeze = 1;
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: freeze\n" );
			break;
		case YPSPUR_UNFREEZE:
			g_spur.freeze = 0;
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: unfreeze\n" );
			break;
		case YPSPUR_ISFREEZE:
			res_msg.data[0] = g_spur.freeze;
			message_return( msq_id, msg.pid, &res_msg );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: isfreeze %d\n", g_spur.freeze );
			break;
		case YPSPUR_VEL:
			vel_com( msg.data, &g_spur );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: vel %f %f\n", g_spur.vref, g_spur.wref );
			break;
		case YPSPUR_WHEEL_VEL:
			wheel_vel_com( msg.data, &g_spur );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: wheelvel %f %f\n", g_spur.vref, g_spur.wref );
			break;
		case YPSPUR_WHEEL_TORQUE:
			set_torque_com( msg.data, &g_spur );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: set_torque %f %f\n", msg.data[0], msg.data[1] );
			break;

	  /*----------command_get.c------------------*/
		case YPSPUR_GET_POS:
			get_pos_com( msg.cs, msg.data, res_msg.data, &g_spur );
			message_return( msq_id, msg.pid, &res_msg );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: get %f %f %f\n", res_msg.data[0], res_msg.data[1], res_msg.data[2] );
			break;
		case YPSPUR_GET_VEL:
			get_vel_com( msg.cs, msg.data, res_msg.data, &g_spur );
			message_return( msq_id, msg.pid, &res_msg );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: getvel %f %f\n", res_msg.data[0], res_msg.data[1] );
			break;
		case YPSPUR_GET_FORCE:
			get_force_com( msg.cs, msg.data, res_msg.data, &g_spur );
			message_return( msq_id, msg.pid, &res_msg );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: getforce %f %f\n", res_msg.data[0], res_msg.data[1] );
			break;
		case YPSPUR_GET_WHEEL_TORQUE:
			get_wheel_torque_com( msg.cs, msg.data, res_msg.data, &g_spur );
			message_return( msq_id, msg.pid, &res_msg );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: getwheeltorque %f %f\n", res_msg.data[0], res_msg.data[1] );
			break;
		case YPSPUR_GET_WHEEL_VEL:
			get_wheel_vel_com( msg.data, res_msg.data, &g_spur );
			message_return( msq_id, msg.pid, &res_msg );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: get wheelvel %f %f\n", g_spur.vref, g_spur.wref );
			break;
		case YPSPUR_GET_WHEEL_ANG:
			get_wheel_ang_com( msg.data, res_msg.data, &g_spur );
			message_return( msq_id, msg.pid, &res_msg );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: get wheelang %f %f\n", g_spur.vref, g_spur.wref );
			break;
		case YPSPUR_NEAR_POS:
			res_msg.cs = near_pos_com( msg.cs, msg.data, res_msg.data, &g_spur );
			message_return( msq_id, msg.pid, &res_msg );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: near pos ( dist = %f )\n", res_msg.data[0] );
			break;
		case YPSPUR_NEAR_ANG:
			res_msg.cs = near_ang_com( msg.cs, msg.data, res_msg.data, &g_spur );
			message_return( msq_id, msg.pid, &res_msg );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: near ang ( dist = %f )\n", res_msg.data[0] );
			break;
		case YPSPUR_OVER_LINE:
			res_msg.cs = over_line_com( msg.cs, msg.data, res_msg.data, &g_spur );
			message_return( msq_id, msg.pid, &res_msg );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: over line ( dist = %f )\n", res_msg.data[0] );
			break;

	  /*-------------command_param.c---------------*/

		case YPSPUR_PARAM_SET:
			param_set_com( msg.cs, msg.data, &g_spur );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: param_set %s %f\n", param_name[msg.cs], msg.data[0] );
			break;

		case YPSPUR_PARAM_GET:
			res_msg.cs = param_get_com( msg.cs, res_msg.data, &g_spur );
			message_return( msq_id, msg.pid, &res_msg );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: param_get %s %f\n", param_name[msg.cs], msg.data[0] );
			break;

		case YPSPUR_PARAM_STATE:
			param_state_com( msg.cs, msg.data, &g_spur );
			break;

	  /*-------------command_ad.c---------------*/

		case YPSPUR_GETAD:
			get_ad_com( msg.data, res_msg.data );
			message_return( msq_id, msg.pid, &res_msg );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: get A/D%d value %d\n", ( int )msg.data[0], ( int )res_msg.data[0] );
			break;

		case YPSPUR_SETIODIR:
			set_io_dir_com( msg.data, res_msg.data );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: set IO dir %d\n", ( int )msg.data[0] );
			break;

		case YPSPUR_SETIODATA:
			set_io_data_com( msg.data, res_msg.data );
			if( output_lv(  ) >= OUTPUT_LV_COMMAND )
				fprintf( stderr, "Command: set IO data %d\n", ( int )msg.data[0] );
			break;

		default:
			if( output_lv(  ) >= OUTPUT_LV_WARNING )
				fprintf( stderr, "Command: unknown\n" );
			break;
		}

		/* 走行モードに変化があったか */
		if( g_spur.run_mode != g_spur.before_run_mode || g_spur.before_freeze != g_spur.freeze )
		{
			if( ( g_spur.run_mode != RUN_FREE && g_spur.run_mode != RUN_WHEEL_TORQUE ) || g_spur.freeze )
			{
				motor_servo(  );
				if( g_spur.freeze )
				{
					if( output_lv(  ) >= OUTPUT_LV_CONTROL )
						fprintf( stderr, "Mode: freeze\n" );
				}
				else
				{
					if( output_lv(  ) >= OUTPUT_LV_CONTROL )
						fprintf( stderr, "Mode: servo %d\n", g_spur.run_mode );
				}
			}
			else if( g_spur.run_mode == RUN_FREE || g_spur.run_mode == RUN_WHEEL_TORQUE )
			{									/* フリーになった */
				motor_free(  );
				if( output_lv(  ) >= OUTPUT_LV_CONTROL )
					fprintf( stderr, "Mode: free\n" );
			}
			// fprintf( stderr, "change\n" );
		}
		g_spur.before_run_mode = g_spur.run_mode;
		g_spur.before_freeze = g_spur.freeze;

		pthread_mutex_unlock( &g_spur.mutex );
		// スレッドの停止要求チェック
		pthread_testcancel(  );
	}

	pthread_cleanup_pop( 1 );
}

/* メッセージを返す */
void message_return( int msq_id, long retpid, YPSpur_msg * res_msg )
{
	res_msg->type = 0;
	res_msg->msg_type = retpid;
	res_msg->pid = 0;
	msgsnd( msq_id, res_msg, YPSPUR_MSG_SIZE, 0 );
}

/* すれっどの初期化 */
void init_command_thread( pthread_t * thread )
{
	if( pthread_create( thread, NULL, ( void * )command, NULL ) != 0 )
	{
		if( output_lv(  ) >= OUTPUT_LV_ERROR )
			fprintf( stderr, "Can't create command thread\n" );
	}
}

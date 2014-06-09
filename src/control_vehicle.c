
/* high level I/O */
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <strings.h>

/* high prec. timer */
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

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

/* yp-spur用 */
#include <serial.h>
#include <param.h>
#include <control.h>
#include <command.h>
#include <utility.h>
#include <yprintf.h>
#include <odometry.h>
#include <ssm_spur_handler.h>

/* ライブラリ用 */
#include <ypspur.h>

#include <pthread.h>


double g_v_ref;
double g_w_ref;
double g_wl_ref;
double g_wr_ref;

/* ホイール速度指令 */
int motor_speed_smooth( double r, double l, SpurUserParamsPtr spur )
{
	double wr, wl;
	double nwl, nwr;
	OdometryPtr podm;

	podm = get_odometry_ptr();
	reference_motor_speed( &nwr, &nwl );

	wl = l;
	if( wl > spur->wheel_vel_l ) wl = spur->wheel_vel_l;
	else if( wl < -spur->wheel_vel_l ) wl = -spur->wheel_vel_l;

	if( wl > nwl + spur->wheel_accel_l * p( YP_PARAM_CONTROL_CYCLE, 0 ) )
	{
		wl = nwl + spur->wheel_accel_l * p( YP_PARAM_CONTROL_CYCLE, 0 );
	}
	else if( wl < nwl - spur->wheel_accel_l * p( YP_PARAM_CONTROL_CYCLE, 0 ) )
	{
		wl = nwl - spur->wheel_accel_l * p( YP_PARAM_CONTROL_CYCLE, 0 );
	}

	wr = r;
	if( wr > spur->wheel_vel_r ) wr = spur->wheel_vel_r;
	else if( wr < -spur->wheel_vel_r ) wr = -spur->wheel_vel_r;

	if( wr > nwr + spur->wheel_accel_r * p( YP_PARAM_CONTROL_CYCLE, 0 ) )
	{
		wr = nwr + spur->wheel_accel_r * p( YP_PARAM_CONTROL_CYCLE, 0 );
	}
	else if( wr < nwr - spur->wheel_accel_r * p( YP_PARAM_CONTROL_CYCLE, 0 ) )
	{
		wr = nwr - spur->wheel_accel_r * p( YP_PARAM_CONTROL_CYCLE, 0 );
	}

	motor_speed( wr, wl );
	g_v_ref = podm->v;
	g_w_ref = podm->w;

	return 0;
}

/* [rad/s] */
void motor_speed( double r, double l )
{
	int ir, il;

	g_wl_ref = l;
	g_wr_ref = r;
	if( option( OPTION_HIGH_PREC ) )
	{
		ir = ( double )( 16 * r * p( YP_PARAM_GEAR, MOTOR_RIGHT ) * p( YP_PARAM_COUNT_REV, MOTOR_RIGHT )
		                  * p( YP_PARAM_CYCLE, MOTOR_RIGHT ) / ( 2 * M_PI ) );
		il = ( double )( 16 * l * p( YP_PARAM_GEAR, MOTOR_LEFT ) * p( YP_PARAM_COUNT_REV, MOTOR_LEFT )
		                  * p( YP_PARAM_CYCLE, MOTOR_LEFT ) / ( 2 * M_PI ) );

		parameter_set( PARAM_w_ref_highprec, 0, ir );
		parameter_set( PARAM_w_ref_highprec, 1, il );
	}
	else
	{
		ir = ( double )( r * p( YP_PARAM_GEAR, MOTOR_RIGHT ) * p( YP_PARAM_COUNT_REV, MOTOR_RIGHT )
		                  * p( YP_PARAM_CYCLE, MOTOR_RIGHT ) / ( 2 * M_PI ) );
		il = ( double )( l * p( YP_PARAM_GEAR, MOTOR_LEFT ) * p( YP_PARAM_COUNT_REV, MOTOR_LEFT )
		                  * p( YP_PARAM_CYCLE, MOTOR_LEFT ) / ( 2 * M_PI ) );

		parameter_set( PARAM_w_ref, 0, ir );
		parameter_set( PARAM_w_ref, 1, il );
	}
}

/* [rad/s] */
void motor_torque( double r, double l )
{
	int ir, il;

	ir = r * p(YP_PARAM_TORQUE_UNIT,MOTOR_RIGHT) / p(YP_PARAM_GEAR,MOTOR_RIGHT);
	il = l * p(YP_PARAM_TORQUE_UNIT,MOTOR_LEFT) / p(YP_PARAM_GEAR,MOTOR_LEFT);
	parameter_set( PARAM_p_toq_offset, 0, ir );
	parameter_set( PARAM_p_toq_offset, 1, il );
}

void update_ref_speed(  )
{
	OdometryPtr podm;

	podm = get_odometry_ptr();
	g_wl_ref = podm->wl;
	g_wr_ref = podm->wr;
	g_v_ref = podm->v;
	g_w_ref = podm->w;
}

/* m/s rad/s */
void robot_speed( double v, double w )
{
	double wr, wl;

	g_v_ref = v;
	g_w_ref = w;

	wr =  ( 0.5 * w * p( YP_PARAM_TREAD, 0 ) + v ) / p( YP_PARAM_RADIUS, MOTOR_RIGHT );
	wl = -( 0.5 * w * p( YP_PARAM_TREAD, 0 ) - v ) / p( YP_PARAM_RADIUS, MOTOR_LEFT );

	motor_speed( wr, wl );
}

int reference_speed( double *v, double *w )
{
	*v = g_v_ref;
	*w = g_w_ref;
	return 0;
}

int reference_motor_speed( double *wr, double *wl )
{
	*wl = g_wl_ref;
	*wr = g_wr_ref;
	return 0;
}

/* - */
int robot_speed_smooth( double v, double w, SpurUserParamsPtr spur )
{
	int limit;
	double dw, dv;

	dw = spur->dw * p( YP_PARAM_CONTROL_CYCLE, 0 );
	dv = spur->dv * p( YP_PARAM_CONTROL_CYCLE, 0 );

	if( fabs( g_v_ref ) > fabs( spur->v ) )
	{
		// 直前の速度が最大速度より大きかったら、ハードウェア最大加速度で減速
		dv = p( YP_PARAM_MAX_ACC_V, 0 ) * p( YP_PARAM_CONTROL_CYCLE, 0 );
	}
	if( fabs( g_w_ref ) > fabs( spur->w ) )
	{
		// 直前の角速度が最大角速度より大きかったら、ハードウェア最大角加速度で減速
		dw = p( YP_PARAM_MAX_ACC_W, 0 ) * p( YP_PARAM_CONTROL_CYCLE, 0 );
	}

	limit = 31;

	if( v > fabs( spur->v ) )
	{
		v = fabs( spur->v );
	}
	else if( v < -fabs( spur->v ) )
	{
		v = -fabs( spur->v );
	}
	else
	{
		limit -= 1;
	}

	if( v > g_v_ref + dv )
	{
		v = g_v_ref + dv;
	}
	else if( v < g_v_ref - dv )
	{
		v = g_v_ref - dv;
	}
	else
	{
		limit -= 2;
	}

	if( w > fabs( spur->w ) )
	{
		w = fabs( spur->w );
	}
	else if( w < -fabs( spur->w ) )
	{
		w = -fabs( spur->w );
	}
	else
	{
		limit -= 4;
	}

	if( w > g_w_ref + dw )
	{
		w = g_w_ref + dw;
	}
	else if( w < g_w_ref - dw )
	{
		w = g_w_ref - dw;
	}
	else
	{
		limit -= 8;
	}

	if( g_w_ref != 0 )
	{
		if( v > p( YP_PARAM_MAX_CENTRIFUGAL_ACC, 0 ) / fabs( g_w_ref ) )
		{
			v = p( YP_PARAM_MAX_CENTRIFUGAL_ACC, 0 ) / fabs( g_w_ref );
		}
		else if( v < -p( YP_PARAM_MAX_CENTRIFUGAL_ACC, 0 ) / fabs( g_w_ref ) )
		{
			v = -p( YP_PARAM_MAX_CENTRIFUGAL_ACC, 0 ) / fabs( g_w_ref );
		}
		else
		{
			limit -= 16;
		}
	}
	else
	{
		limit -= 16;
	}

	robot_speed( v, w );
	return limit;
}

/* 重力補償 */
double gravity_compensation( OdometryPtr odm, SpurUserParamsPtr spur )
{
	double tilt, f;

	/* 傾きを計算 */
	tilt = atan( cos( odm->theta - spur->dir ) * tan( spur->tilt ) );
	/* 力を計算 */
	f = p( YP_PARAM_MASS, 0 ) * GRAVITY * sin( tilt );
	/* [N]=[kg]*[m/ss]*tilt */
	/* [Nm] [N]* [m] /[in/out] */
	spur->grav_torque_r = f * p( YP_PARAM_RADIUS, MOTOR_RIGHT ) / 2.0;
	spur->grav_torque_l = f * p( YP_PARAM_RADIUS, MOTOR_LEFT ) / 2.0;
	yprintf( OUTPUT_LV_DEBUG, "Force:%f Torque:%f/%f\n", f, spur->grav_torque_r, spur->grav_torque_l );
	return tilt;
}

void control_loop_cleanup( void *data )
{
	yprintf( OUTPUT_LV_MODULE, "Trajectory control loop stopped.\n" );
}

/* 20msごとの割り込みで軌跡追従制御処理を呼び出す */
void control_loop( void )
{
	OdometryPtr odometry;
	SpurUserParamsPtr spur;

	odometry = get_odometry_ptr(  );
	spur = get_spur_user_param_ptr(  );

	yprintf( OUTPUT_LV_MODULE, "Trajectory control loop started.\n" );
	pthread_cleanup_push( control_loop_cleanup, NULL );

#if defined(HAVE_LIBRT)							// clock_nanosleepが利用可能
	struct timespec request;

	if( clock_gettime( CLOCK_MONOTONIC, &request ) == -1 )
	{
		yprintf( OUTPUT_LV_ERROR, "error on clock_gettime\n" );
		exit( 0 );
	}
	while( 1 )
	{
		request.tv_nsec += ( p( YP_PARAM_CONTROL_CYCLE, 0 ) * 1000000000 );
		request.tv_sec += request.tv_nsec / 1000000000;
		request.tv_nsec = request.tv_nsec % 1000000000;

		clock_nanosleep( CLOCK_MONOTONIC, TIMER_ABSTIME, &request, 0 );
		coordinate_synchronize( odometry, spur );
		run_control( *odometry, spur );

		// スレッドの停止要求チェック
		pthread_testcancel(  );
	}
#else
	int request;
	request = ( p( YP_PARAM_CONTROL_CYCLE, 0 ) * 1000000 );

	while( 1 )
	{
		yp_usleep( request );
		coordinate_synchronize( odometry, spur );
		run_control( *odometry, spur );

		// スレッドの停止要求チェック
		pthread_testcancel(  );
	}
#endif
	pthread_cleanup_pop( 1 );
}

/* 追従軌跡に応じた処理 */
void run_control( Odometry odometry, SpurUserParamsPtr spur )
{
	static double before_time = 0;
	double now_time;

	now_time = get_time(  );
	// printf( "%f %f\n", now_time - before_time );fflush(stdout);
	spur->control_dt = now_time - before_time;

	before_time = now_time;
	if( before_time == 0 )
		return;

	/* パラメータの変更がおこらないようにブロック */
	pthread_mutex_lock( &spur->mutex );

	cstrans_odometry( CS_SP, &odometry );

	if( spur->freeze )
	{
		SpurUserParams spur_freeze;

		spur_freeze = *spur;
		spur_freeze.v = p( YP_PARAM_MAX_VEL, 0 );
		spur_freeze.w = p( YP_PARAM_MAX_W, 0 );
		spur_freeze.dv = p( YP_PARAM_MAX_ACC_V, 0 );
		spur_freeze.dw = p( YP_PARAM_MAX_ACC_W, 0 );
		robot_speed_smooth( 0, 0, &spur_freeze );
	}
	else
	{
		int is_vehicle_control;
		float torque_ref[ 2 ] = { 0, 0 };
		int torque_set = 0;

		/* 重力補償 */
		if( state( YP_STATE_GRAVITY ) )
		{
			gravity_compensation( &odometry, spur );
			torque_ref[ 0 ] = spur->torque_r;
			torque_ref[ 1 ] = spur->torque_l;
			torque_set = 1;
		}
		else
		{
			if( isset_p( YP_PARAM_TORQUE_OFFSET, 0 ) && isset_p( YP_PARAM_TORQUE_OFFSET, 1 ) )
			{
				torque_ref[ 0 ] = p( YP_PARAM_TORQUE_OFFSET, 0 );
				torque_ref[ 1 ] = p( YP_PARAM_TORQUE_OFFSET, 1 );
				torque_set = 1;
			}
		}

		is_vehicle_control = 0;
		/* 走行状態に応じた処理 */
		switch ( spur->run_mode )
		{
		case RUN_OPENFREE:
			torque_set = 1;
			update_ref_speed(  );
			break;
		case RUN_FREE:
			torque_set = 1;
			update_ref_speed(  );
			break;
		case RUN_STOP:							// ストップする（スピードを0にする）
			robot_speed_smooth( 0, 0, spur );
			is_vehicle_control = 1;
			break;
		case RUN_WHEEL_VEL:					// 速度直接指定
			if( state( YP_STATE_BODY ) == ENABLE )
				motor_speed_smooth( spur->wrref, spur->wlref, spur );
			is_vehicle_control = 1;
			break;
		case RUN_WHEEL_TORQUE:					// トルク直接指定
			torque_set = 1;
			torque_ref[ 0 ] += spur->torque_r;
			torque_ref[ 1 ] += spur->torque_l;
			update_ref_speed(  );
			break;
		case RUN_VEL:							// 速度角速度指定
			if( state( YP_STATE_BODY ) == ENABLE )
				robot_speed_smooth( spur->vref, spur->wref, spur );
			is_vehicle_control = 1;
			break;
		case RUN_LINEFOLLOW:					// 直線追従
			if( state( YP_STATE_BODY ) == ENABLE && state( YP_STATE_TRACKING ) == ENABLE )
				line_follow( &odometry, spur );
			is_vehicle_control = 1;
			break;
		case RUN_STOP_LINE:					// 短辺への移動
			if( state( YP_STATE_BODY ) == ENABLE && state( YP_STATE_TRACKING ) == ENABLE )
				stop_line( &odometry, spur );
			is_vehicle_control = 1;
			break;
		case RUN_CIRCLEFOLLOW:					// 円弧追従
			if( state( YP_STATE_BODY ) == ENABLE && state( YP_STATE_TRACKING ) == ENABLE )
				circle_follow( &odometry, spur );
			is_vehicle_control = 1;
			break;
		case RUN_SPIN:							// 回転
			if( state( YP_STATE_BODY ) == ENABLE && state( YP_STATE_TRACKING ) == ENABLE )
				spin( &odometry, spur );
			is_vehicle_control = 1;
			break;
		case RUN_ORIENT:						// 方位
			if( state( YP_STATE_BODY ) == ENABLE && state( YP_STATE_TRACKING ) == ENABLE )
				orient( &odometry, spur );
			is_vehicle_control = 1;
			break;
		case RUN_WHEEL_ANGLE:						// 回転
			if( state( YP_STATE_BODY ) == ENABLE )
				wheel_angle( &odometry, spur );
			is_vehicle_control = 1;
			break;
		}
		if( torque_set )
		{
			motor_torque( torque_ref[ 0 ], torque_ref[ 1 ] );
		}
		if( is_vehicle_control )
		{
		}
	}
	/* 保護終わり */
	pthread_mutex_unlock( &spur->mutex );

}

/* すれっどの初期化 */
void init_control_thread( pthread_t * thread )
{
	if( pthread_create( thread, NULL, ( void * )control_loop, NULL ) != 0 )
	{
		yprintf( OUTPUT_LV_ERROR, "Can't create control_loop thread\n" );
	}
}


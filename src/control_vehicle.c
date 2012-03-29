
/* high level I/O */
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <strings.h>
#include <pthread.h>

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
#include <odometry.h>
#include <ssm_spur_handler.h>

/* ライブラリ用 */
#include <ypspur.h>

double g_v_ref;
double g_w_ref;
double g_cnt2rad;


/* [rad/s] */
void motor_speed( double r, double l )
{
	int ir, il;

	ir = ( double )( r * g_cnt2rad );
	il = ( double )( l * g_cnt2rad );

	parameter_set( PARAM_w_ref, 0, ir );
	parameter_set( PARAM_w_ref, 1, il );

	// printf("%f %f\n",r,l);
}

/* m/s rad/s */
void robot_speed( double v, double w )
{
	if( option( OPTION_PASSIVE ) )
	{
		double tr, tl;
		double v2, w2, werr;
		double vover;
		static double werr_int = 0.0;
		OdometryPtr podm;

		podm = get_odometry_ptr();
		v2 = podm->v;
		g_v_ref = v;
		g_w_ref = w;
		//printf("vref %f wref%f ",v, w );
		
		if( fabs( v2 ) < 0.01 || fabs( v ) < 0.01 )
		{
			w2 = 0;
		}
		else
		{
			w2 = ( w * v2 ) / v;
		}

		vover = 0;
		if( v >= 0 )
		{
			if( v2 > v ) vover = v2 - v;
			else if( v2 < -0.05 ) vover = v2 + 0.05;
		}
		else
		{
			if( v2 < v ) vover = v2 - v;
			else if( v2 > 0.05 ) vover = v2 - 0.05;
		}

		werr = ( w2 - podm->w );
		werr_int += werr * p( YP_PARAM_CONTROL_CYCLE );

		if( werr_int > 50  ) werr_int = 50;
		if( werr_int < -50 ) werr_int = -50;

		tr = - ( vover * vover * SIGN(vover) ) * 50 * p(YP_PARAM_MASS)
			 + ( -( werr * 20 + werr_int * 30 ) * p(YP_PARAM_MOMENT_INERTIA) );
		tl = - ( vover * vover * SIGN(vover) ) * 50 * p(YP_PARAM_MASS)
			 - ( -( werr * 20 + werr_int * 30 ) * p(YP_PARAM_MOMENT_INERTIA) );
		tr *= ( ( p(YP_PARAM_RADIUS_R) + p(YP_PARAM_RADIUS_L) ) / 2.0 ) / p(YP_PARAM_GEAR);
		tl *= ( ( p(YP_PARAM_RADIUS_R) + p(YP_PARAM_RADIUS_L) ) / 2.0 ) / p(YP_PARAM_GEAR);
		//printf("v%f w%f %f %f   %f %f c%f vo%f\n",v2,w2,tr,tl,werr,werr_int, p( YP_PARAM_CONTROL_CYCLE ),vover);

		parameter_set( PARAM_p_toq_offset, 0, tr * p(YP_PARAM_TORQUE_UNIT) );
		parameter_set( PARAM_p_toq_offset, 1, tl * p(YP_PARAM_TORQUE_UNIT) );
	}
	else
	{
		double wr, wl;

		g_v_ref = v;
		g_w_ref = w;

		wr = -( 0.5 * w * p( YP_PARAM_TREAD ) - v ) / p( YP_PARAM_RADIUS_R );
		wl = ( 0.5 * w * p( YP_PARAM_TREAD ) + v ) / p( YP_PARAM_RADIUS_L );

		motor_speed( wr, wl );
	}
}

int reference_speed( double *v, double *w )
{
	*v = g_v_ref;
	*w = g_w_ref;
	return 0;
}

/* - */
int robot_speed_smooth( double v, double w, SpurUserParamsPtr spur )
{
	int limit;

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

	if( v > g_v_ref + spur->dv * p( YP_PARAM_CONTROL_CYCLE ) )
	{
		v = g_v_ref + spur->dv * p( YP_PARAM_CONTROL_CYCLE );
	}
	else if( v < g_v_ref - spur->dv * p( YP_PARAM_CONTROL_CYCLE ) )
	{
		v = g_v_ref - spur->dv * p( YP_PARAM_CONTROL_CYCLE );
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

	if( w > g_w_ref + spur->dw * p( YP_PARAM_CONTROL_CYCLE ) )
	{
		w = g_w_ref + spur->dw * p( YP_PARAM_CONTROL_CYCLE );
	}
	else if( w < g_w_ref - spur->dw * p( YP_PARAM_CONTROL_CYCLE ) )
	{
		w = g_w_ref - spur->dw * p( YP_PARAM_CONTROL_CYCLE );
	}
	else
	{
		limit -= 8;
	}

	if( g_w_ref != 0 )
	{
		if( v > p( YP_PARAM_MAX_CENTRIFUGAL_ACC ) / fabs( g_w_ref ) )
		{
			v = p( YP_PARAM_MAX_CENTRIFUGAL_ACC ) / fabs( g_w_ref );
		}
		else if( v < -p( YP_PARAM_MAX_CENTRIFUGAL_ACC ) / fabs( g_w_ref ) )
		{
			v = -p( YP_PARAM_MAX_CENTRIFUGAL_ACC ) / fabs( g_w_ref );
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
	f = p( YP_PARAM_MASS ) * GRAVITY * sin( tilt );
	/* [N]=[kg]*[m/ss]*tilt */
	/* トルクを計算 */
	*pp( YP_PARAM_TORQUE_OFFSET ) = f * p( YP_PARAM_RADIUS_R ) / p( YP_PARAM_GEAR );
	/* [Nm] [N]* [m] /[in/out] */
	if( output_lv(  ) >= OUTPUT_LV_DEBUG )
		printf( "%f %f\n", f, p( YP_PARAM_TORQUE_OFFSET ) * p( YP_PARAM_TORQUE_UNIT ) / 2.0 );
	parameter_set( PARAM_p_toq_offset, 0, p( YP_PARAM_TORQUE_OFFSET ) * p( YP_PARAM_TORQUE_UNIT ) / 2.0 );
	parameter_set( PARAM_p_toq_offset, 1, p( YP_PARAM_TORQUE_OFFSET ) * p( YP_PARAM_TORQUE_UNIT ) / 2.0 );
	return tilt;
}

void control_loop_cleanup( void *data )
{
	if( output_lv(  ) >= OUTPUT_LV_MODULE )
		fprintf( stderr, "Trajectory control loop stopped.\n" );
}

/* 20msごとの割り込みで軌跡追従制御処理を呼び出す */
void control_loop( void )
{
	OdometryPtr odometry;
	SpurUserParamsPtr spur;

	odometry = get_odometry_ptr(  );
	spur = get_spur_user_param_ptr(  );

	if( output_lv(  ) >= OUTPUT_LV_MODULE )
		fprintf( stderr, "Trajectory control loop started.\n" );
	pthread_cleanup_push( control_loop_cleanup, NULL );

#if defined(HAVE_LIBRT)							// clock_nanosleepが利用可能
	struct timespec request;

	if( clock_gettime( CLOCK_MONOTONIC, &request ) == -1 )
	{
		if( output_lv(  ) >= OUTPUT_LV_ERROR )
			fprintf( stderr, "error on clock_gettime\n" );
		exit( 0 );
	}
	while( 1 )
	{
		request.tv_nsec += ( p( YP_PARAM_CONTROL_CYCLE ) * 1000000000 );
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
	request = ( p( YP_PARAM_CONTROL_CYCLE ) * 1000000 );

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

	before_time = now_time;						// +=0.02;
	if( before_time == 0 )
		return;

	/* パラメータの変更がおこらないようにブロック */
	pthread_mutex_lock( &spur->mutex );

	cstrans_odometry( CS_SP, &odometry );

	if( spur->freeze )
	{
		SpurUserParams spur_freeze;

		spur_freeze = *spur;
		spur_freeze.v = p( YP_PARAM_MAX_VEL );
		spur_freeze.w = p( YP_PARAM_MAX_W );
		spur_freeze.dv = p( YP_PARAM_MAX_ACC_V );
		spur_freeze.dw = p( YP_PARAM_MAX_ACC_W );
		robot_speed_smooth( 0, 0, &spur_freeze );
	}
	else
	{
		/* 重力補償 */
		if( state( YP_STATE_GRAVITY ) )
		{
			gravity_compensation( &odometry, spur );
		}

		/* 走行状態に応じた処理 */
		switch ( spur->run_mode )
		{
		case RUN_FREE:
			//robot_speed_smooth( 0, 0, spur );
			parameter_set( PARAM_p_toq_offset, 0, 0 );
			parameter_set( PARAM_p_toq_offset, 1, 0 );
			{
				OdometryPtr podm;

				podm = get_odometry_ptr();
				g_v_ref = podm->v;
				g_w_ref = podm->w;
			}
			break;
		case RUN_STOP:							// ストップする（スピードを0にする）
			robot_speed_smooth( 0, 0, spur );
			break;
		case RUN_WHEEL_VEL:					// 速度直接指定
			motor_speed( spur->vref, spur->wref );
			break;
		case RUN_WHEEL_TORQUE:					// 速度直接指定
			parameter_set( PARAM_p_toq_offset, 0, spur->torque_l * p(YP_PARAM_TORQUE_UNIT) / p(YP_PARAM_GEAR) );
			parameter_set( PARAM_p_toq_offset, 1, spur->torque_r * p(YP_PARAM_TORQUE_UNIT) / p(YP_PARAM_GEAR) );
			break;
		case RUN_VEL:							// 速度角速度指定
			if( state( YP_STATE_BODY ) == ENABLE )
				robot_speed_smooth( spur->vref, spur->wref, spur );
			break;
		case RUN_LINEFOLLOW:					// 直線追従
			if( state( YP_STATE_BODY ) == ENABLE && state( YP_STATE_TRACKING ) == ENABLE )
				line_follow( &odometry, spur );
			break;
		case RUN_STOP_LINE:					// 短辺への移動
			if( state( YP_STATE_BODY ) == ENABLE && state( YP_STATE_TRACKING ) == ENABLE )
				stop_line( &odometry, spur );
			break;
		case RUN_CIRCLEFOLLOW:					// 円弧追従
			if( state( YP_STATE_BODY ) == ENABLE && state( YP_STATE_TRACKING ) == ENABLE )
				circle_follow( &odometry, spur );
			break;
		case RUN_SPIN:							// 回転
			if( state( YP_STATE_BODY ) == ENABLE && state( YP_STATE_TRACKING ) == ENABLE )
				spin( &odometry, spur );
			break;
		case RUN_ORIENT:						// 方位
			if( state( YP_STATE_BODY ) == ENABLE && state( YP_STATE_TRACKING ) == ENABLE )
				orient( &odometry, spur );
			break;
		}
	}
	/* 保護終わり */
	pthread_mutex_unlock( &spur->mutex );

}

/* すれっどの初期化 */
void init_control_thread( pthread_t * thread )
{
	g_cnt2rad = p( YP_PARAM_GEAR ) * p( YP_PARAM_COUNT_REV ) * p( YP_PARAM_CYCLE ) / ( 2 * M_PI );

	if( pthread_create( thread, NULL, ( void * )control_loop, NULL ) != 0 )
	{
		if( output_lv(  ) >= OUTPUT_LV_ERROR )
			fprintf( stderr, "Can't create control_loop thread\n" );
	}
}


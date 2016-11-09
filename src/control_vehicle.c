
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


/* ホイール速度指令 */
int motor_control( SpurUserParamsPtr spur )
{
	int i;
	ParametersPtr param;
	OdometryPtr podm;

	param = get_param_ptr();
	podm = get_odometry_ptr();

	for( i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i++)
	{
		double ref;
		if( !param->motor_enable[i] ) continue;
		
		switch( spur->wheel_mode[i] )
		{
		case MOTOR_CONTROL_OPENFREE:
		case MOTOR_CONTROL_FREE:
			break;
		case MOTOR_CONTROL_VEHICLE:
			break;
		case MOTOR_CONTROL_ANGLE:
			spur->wvelref[i] = timeoptimal_servo( 
					podm->wang[i] - spur->wheel_angle[i], 
					spur->wheel_vel[i],
					podm->wvel[i],
					spur->wheel_accel[i] );
		case MOTOR_CONTROL_VEL:
			ref = spur->wvelref[i];
			if( ref > spur->wheel_vel[i] ) ref = spur->wheel_vel[i];
			else if( ref < -spur->wheel_vel[i] ) ref = -spur->wheel_vel[i];

			if( ref > spur->wheel_vel_smooth[i] + spur->wheel_accel[i] * p( YP_PARAM_CONTROL_CYCLE, i ) )
			{
				ref = spur->wheel_vel_smooth[i] + spur->wheel_accel[i] * p( YP_PARAM_CONTROL_CYCLE, i );
			}
			else if( ref < spur->wheel_vel_smooth[i] - spur->wheel_accel[i] * p( YP_PARAM_CONTROL_CYCLE, i ) )
			{
				ref = spur->wheel_vel_smooth[i] - spur->wheel_accel[i] * p( YP_PARAM_CONTROL_CYCLE, i );
			}
			spur->wheel_vel_smooth[i] = ref;
			break;
		}
	}

	return 0;
}

/* [rad/s] */
void apply_motor_speed( SpurUserParamsPtr spur )
{
	int i;
	ParametersPtr param;
	param = get_param_ptr();
	spur->run_mode_cnt ++;

	for( i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i ++ )
	{
		int v;
		if( !param->motor_enable[i] ) continue;

		int force_send_control_mode = 0;
		if( p(YP_PARAM_CONTROL_MODE_RESEND, i) != 0.0 )
		{
			int cnt_max = p(YP_PARAM_CONTROL_MODE_RESEND, i) / p(YP_PARAM_CONTROL_CYCLE, i);
			if( spur->run_mode_cnt % cnt_max == i % cnt_max )
				force_send_control_mode = 1;
		}

		if( spur->wheel_mode[i] != spur->wheel_mode_prev[i] || force_send_control_mode )
		{
			switch( spur->wheel_mode[i] )
			{
			case MOTOR_CONTROL_OPENFREE:
				parameter_set( PARAM_servo, i, SERVO_LEVEL_OPENFREE );
				break;
			case MOTOR_CONTROL_FREE:
				parameter_set( PARAM_servo, i, SERVO_LEVEL_TORQUE );
				break;
			case MOTOR_CONTROL_ANGLE:
			case MOTOR_CONTROL_VEL:
			case MOTOR_CONTROL_VEHICLE:
				parameter_set( PARAM_servo, i, SERVO_LEVEL_VELOCITY );
				break;
			}
		}
		spur->wheel_mode_prev[i] = spur->wheel_mode[i];

		switch( spur->wheel_mode[i] )
		{
		case MOTOR_CONTROL_OPENFREE:
		case MOTOR_CONTROL_FREE:
			parameter_set( PARAM_w_ref, i, 0 );
			break;
		case MOTOR_CONTROL_ANGLE:
		case MOTOR_CONTROL_VEL:
		case MOTOR_CONTROL_VEHICLE:
			if( option( OPTION_HIGH_PREC ) )
			{
				v = ( double )( 16 * spur->wheel_vel_smooth[i] * p( YP_PARAM_GEAR, i ) * p( YP_PARAM_COUNT_REV, i )
						* p( YP_PARAM_CYCLE, i ) / ( 2 * M_PI ) );
				parameter_set( PARAM_w_ref_highprec, i, v );
			}
			else
			{
				v = ( double )( spur->wheel_vel_smooth[i] * p( YP_PARAM_GEAR, i ) * p( YP_PARAM_COUNT_REV, i )
						* p( YP_PARAM_CYCLE, i ) / ( 2 * M_PI ) );
				parameter_set( PARAM_w_ref, i, v );
			}
			break;
		}
	}
}

/* [rad/s] */
void apply_motor_torque( SpurUserParamsPtr spur )
{
	int i;
	ParametersPtr param;
	param = get_param_ptr();

	for( i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i ++ )
	{
		if( !param->motor_enable[i] ) continue;

		if( spur->torque[i] != spur->torque_prev[i] )
		{
			int t;
			t = spur->torque[i] * p(YP_PARAM_TORQUE_UNIT, i) / p(YP_PARAM_GEAR,MOTOR_RIGHT);
			parameter_set( PARAM_p_toq_offset, i, t );
		}
		spur->torque_prev[i] = spur->torque[i];
	}
}

void update_ref_speed( SpurUserParamsPtr spur )
{
	OdometryPtr podm;
	int i;

	podm = get_odometry_ptr();
	for( i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i ++ )
	{
		spur->wheel_vel_smooth[i] = podm->wvel[i];
	}
	spur->vref_smooth = podm->v;
	spur->wref_smooth = podm->w;
}

/* m/s rad/s */
void robot_speed( SpurUserParamsPtr spur )
{
	int i;
	int vc_i;

	ParametersPtr param;
	param = get_param_ptr();

	vc_i = 0;
	for( i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i ++ )
	{
		if( !param->motor_enable[i] ) continue;

		if( p( YP_PARAM_VEHICLE_CONTROL, i ) > 0 )
		{
			spur->wheel_vel_smooth[i] = 0;
			spur->wheel_mode[i] = MOTOR_CONTROL_VEHICLE;

			switch(vc_i)
			{
			case 0:
				spur->wheel_vel_smooth[i] =  ( 0.5 * spur->wref_smooth * p( YP_PARAM_TREAD, 0 )
						+ spur->vref_smooth ) / p( YP_PARAM_RADIUS, i );
				break;
			case 1:
				spur->wheel_vel_smooth[i] = -( 0.5 * spur->wref_smooth * p( YP_PARAM_TREAD, 0 )
						- spur->vref_smooth ) / p( YP_PARAM_RADIUS, i );
				break;
			default:
				break;
			}
			vc_i ++;
		}
	}
}

void wheel_vel( OdometryPtr odm, SpurUserParamsPtr spur )
{
	int i;
	ParametersPtr param;
	param = get_param_ptr();

	for( i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i ++ )
	{
		if( !param->motor_enable[i] ) continue;

		if( p( YP_PARAM_VEHICLE_CONTROL, i ) > 0 )
		{
			spur->wheel_mode[i] = MOTOR_CONTROL_VEL;
		}
	}
}

void wheel_angle( OdometryPtr odm, SpurUserParamsPtr spur )
{
	int i;
	ParametersPtr param;
	param = get_param_ptr();

	for( i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i ++ )
	{
		if( !param->motor_enable[i] ) continue;

		if( p( YP_PARAM_VEHICLE_CONTROL, i ) > 0 )
		{
			spur->wheel_mode[i] = MOTOR_CONTROL_ANGLE;
		}
	}
}

void wheel_torque( OdometryPtr odm, SpurUserParamsPtr spur, double *torque )
{
	int i;
	ParametersPtr param;
	param = get_param_ptr();

	for( i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i ++ )
	{
		if( !param->motor_enable[i] ) continue;

		if( p( YP_PARAM_VEHICLE_CONTROL, i ) > 0 )
		{
			torque[ i ] += spur->torque[i];
			spur->wheel_mode[i] = MOTOR_CONTROL_FREE;
		}
	}
}


/* - */
int robot_speed_smooth( SpurUserParamsPtr spur )
{
	int limit;
	double dw, dv;
	double v, w;

	v = spur->vref;
	w = spur->wref;

	dw = spur->dw * p( YP_PARAM_CONTROL_CYCLE, 0 );
	dv = spur->dv * p( YP_PARAM_CONTROL_CYCLE, 0 );

	if( fabs( spur->vref_smooth ) > fabs( spur->v ) )
	{
		// 直前の速度が最大速度より大きかったら、ハードウェア最大加速度で減速
		dv = p( YP_PARAM_MAX_ACC_V, 0 ) * p( YP_PARAM_CONTROL_CYCLE, 0 );
	}
	if( fabs( spur->wref_smooth ) > fabs( spur->w ) )
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

	if( v > spur->vref_smooth + dv )
	{
		v = spur->vref_smooth + dv;
	}
	else if( v < spur->vref_smooth - dv )
	{
		v = spur->vref_smooth - dv;
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

	if( w > spur->wref_smooth + dw )
	{
		w = spur->wref_smooth + dw;
	}
	else if( w < spur->wref_smooth - dw )
	{
		w = spur->wref_smooth - dw;
	}
	else
	{
		limit -= 8;
	}

	if( spur->wref_smooth != 0 )
	{
		if( v > p( YP_PARAM_MAX_CENTRIFUGAL_ACC, 0 ) / fabs( spur->wref_smooth ) )
		{
			v = p( YP_PARAM_MAX_CENTRIFUGAL_ACC, 0 ) / fabs( spur->wref_smooth );
		}
		else if( v < -p( YP_PARAM_MAX_CENTRIFUGAL_ACC, 0 ) / fabs( spur->wref_smooth ) )
		{
			v = -p( YP_PARAM_MAX_CENTRIFUGAL_ACC, 0 ) / fabs( spur->wref_smooth );
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

	spur->vref_smooth = v;
	spur->wref_smooth = w;
	robot_speed( spur );
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
	spur->grav_torque[0] = f * p( YP_PARAM_RADIUS, MOTOR_RIGHT ) / 2.0;
	spur->grav_torque[1] = f * p( YP_PARAM_RADIUS, MOTOR_LEFT ) / 2.0;
	yprintf( OUTPUT_LV_DEBUG, "Force:%f Torque:%f/%f\n", f, spur->grav_torque[0], spur->grav_torque[1] );
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
	ParametersPtr param;

	param = get_param_ptr();

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
		int i;

		spur_freeze = *spur;
		spur_freeze.v = p( YP_PARAM_MAX_VEL, 0 );
		spur_freeze.w = p( YP_PARAM_MAX_W, 0 );
		spur_freeze.dv = p( YP_PARAM_MAX_ACC_V, 0 );
		spur_freeze.dw = p( YP_PARAM_MAX_ACC_W, 0 );
		spur_freeze.run_mode = RUN_VEL;
		spur_freeze.vref = 0;
		spur_freeze.wref = 0;
		for( i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i++ )
		{
			if( !param->motor_enable[i] ) continue;
			spur_freeze.wheel_vel[i] = 0;
		}
		robot_speed_smooth( &spur_freeze );
	}
	else
	{
		double torque_ref[ YP_PARAM_MAX_MOTOR_NUM ];
		int i;

		for( i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i ++ )
		{
			torque_ref[ i ] = 0;
		}
		/* 重力補償 */
		if( state( YP_STATE_GRAVITY ) )
		{
			gravity_compensation( &odometry, spur );
		}
		for( i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i ++ )
		{
			if( !param->motor_enable[i] ) continue;

			if( isset_p( YP_PARAM_TORQUE_OFFSET, i ) )
			{
				torque_ref[i] += p( YP_PARAM_TORQUE_OFFSET, 0 );
			}
			if( state( YP_STATE_GRAVITY ) )
			{
				torque_ref[i] += spur->grav_torque[i];
			}
		}

		/* 走行状態に応じた処理 */
		switch ( spur->run_mode )
		{
		case RUN_OPENFREE:
			update_ref_speed( spur );
			break;
		case RUN_FREE:
			update_ref_speed( spur );
			break;
		case RUN_STOP:							// ストップする（スピードを0にする）
			spur->vref = 0;
			spur->wref = 0;
			break;
		case RUN_WHEEL_VEL:					// 速度直接指定
			if( state( YP_STATE_BODY ) == ENABLE )
				wheel_vel( &odometry, spur );
			break;
		case RUN_WHEEL_TORQUE:					// トルク直接指定
			wheel_torque( &odometry, spur, torque_ref );
			update_ref_speed( spur );
			break;
		case RUN_VEL:							// 速度角速度指定
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
		case RUN_WHEEL_ANGLE:						// 回転
			if( state( YP_STATE_BODY ) == ENABLE )
				wheel_angle( &odometry, spur );
			break;
		}
		switch ( spur->run_mode )
		{
		case RUN_VEL:							// 速度角速度指定
		case RUN_STOP:							// ストップする（スピードを0にする）
		case RUN_LINEFOLLOW:					// 直線追従
		case RUN_STOP_LINE:					// 短辺への移動
		case RUN_CIRCLEFOLLOW:					// 円弧追従
		case RUN_SPIN:							// 回転
		case RUN_ORIENT:						// 方位
			if( state( YP_STATE_BODY ) == ENABLE )
				robot_speed_smooth( spur );
			break;
		default:
			break;
		}
		apply_motor_torque( spur );
		motor_control( spur );
		apply_motor_speed( spur );
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


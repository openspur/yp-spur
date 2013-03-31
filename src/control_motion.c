
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

/* ライブラリ用 */
#include <ypspur.h>

/*-PI < theta < PIに調整する*/
double trans_q( double theta )
{
	while( theta > M_PI )
		theta -= 2.0 * M_PI;
	while( theta < -M_PI )
		theta += 2.0 * M_PI;
	return theta;
}

/* 円弧追従 */
double circle_follow( OdometryPtr odm, SpurUserParamsPtr spur )
{
	double d, q, r, ang, rad;

	r = sqrt( ( spur->x - odm->x ) * ( spur->x - odm->x ) + ( spur->y - odm->y ) * ( spur->y - odm->y ) );

	ang = atan2( ( odm->y - spur->y ), ( odm->x - spur->x ) );
	ang = trans_q( ang );

	// レギュレータ問題に変換
	d = fabs( spur->radius ) - r;
	q = trans_q( odm->theta - ( ang + SIGN( spur->radius ) * ( M_PI / 2.0 ) ) );

	if( r < fabs( spur->radius ) )
	{
		rad = spur->radius;
	}
	else
	{
		rad = SIGN( spur->radius ) * r;
	}

	return regurator( d, q, rad, spur->v, spur->w, spur );
}

/* 直線追従 */
double line_follow( OdometryPtr odm, SpurUserParamsPtr spur )
{
	double d, q;

	d = ( spur->x - odm->x ) * sin( spur->theta ) - ( spur->y - odm->y ) * cos( spur->theta );
	q = odm->theta - spur->theta;
	q = trans_q( q );

	return regurator( d, q, 1000, spur->v, spur->w, spur );
	// 1000は無限大のつもり(1km)
}

/* 軌跡追従レギュレータ */
double regurator( double d, double q, double r, double v_max, double w_max, SpurUserParamsPtr spur )
{
	double nv, nw;
	double v, w;
	double cd;
	double wref;

	reference_speed( &nv, &nw );

	v = v_max - SIGN( v_max ) * p( YP_PARAM_L_C1, 0 ) * fabs( nw );
	wref = v_max / r;
	if( wref > fabs( w_max ) )
		wref = fabs( w_max );
	else if( wref < -fabs( w_max ) )
		wref = -fabs( w_max );

	// printf("d%f q%f r%f v%f w%f wref%f\n",d,q,r,nv,nw,wref);fflush(stdout);
	cd = d;
	if( cd > p( YP_PARAM_L_DIST, 0 ) )
		cd = p( YP_PARAM_L_DIST, 0 );
	if( cd < -p( YP_PARAM_L_DIST, 0 ) )
		cd = -p( YP_PARAM_L_DIST, 0 );
	w = nw -
		spur->control_dt * ( SIGN( r ) * SIGN( v_max ) * p( YP_PARAM_L_K1, 0 ) * cd +
							 p( YP_PARAM_L_K2, 0 ) * q + p( YP_PARAM_L_K3, 0 ) * ( nw - wref ) );
	// 円弧追従の制御を正しく[AWD]

	v = v_max;

	robot_speed_smooth( v, w, spur );
	return d;
}

/* 回転 */
double spin( OdometryPtr odm, SpurUserParamsPtr spur )
{
	double q, w_limit;
	double w;
	double nw, nv;
	double delay;

	delay = spur->control_dt
		 + 2 * ( 1 / p(YP_PARAM_GAIN_KP,0) + 1 / p(YP_PARAM_GAIN_KP,1) ) / 2;

	reference_speed( &nv, &nw );

	q = ( odm->theta + nw * delay ) - spur->theta;
	q = trans_q( q );

	/* 次の制御周期で停止するのに限界の速度を計算 */
	w_limit = sqrt( 2 * spur->dw * fabs( q ) );

	if( spur->w < w_limit )
	{
		w = -SIGN( q ) * fabs( spur->w );
	}
	else
	{
		w = -SIGN( q ) * w_limit;
	}
	if( fabs( q ) < p(YP_PARAM_SPIN_LINEAR,0) )
	{
		w = -sqrt( 2 * spur->dw * p(YP_PARAM_SPIN_LINEAR,0) ) * q / p(YP_PARAM_SPIN_LINEAR,0);
	}

	robot_speed_smooth( 0, w, spur );
	return fabs( q );
}

/* 方角 */
double orient( OdometryPtr odm, SpurUserParamsPtr spur )
{
	double q;

	q = odm->theta - spur->theta;
	q = trans_q( q );

	return regurator( 0, q, 1000, spur->v, spur->w, spur );
	// 1000は無限大のつもり(1km)
}

/* 点までの距離 */
double dist_pos( OdometryPtr odm, SpurUserParamsPtr spur )
{
	double r;
	r = sqrt( ( spur->x - odm->x ) * ( spur->x - odm->x ) + ( spur->y - odm->y ) * ( spur->y - odm->y ) );

	return r;
}

/* 直線まで移動し止まる */
int stop_line( OdometryPtr odm, SpurUserParamsPtr spur )
{
	double a;
	double q;
	double vel;
	int over;
	double x, y, nw, nv;
	double delay;

	delay = spur->control_dt
		 + 2 * ( 1 / p(YP_PARAM_GAIN_KP,0) + 1 / p(YP_PARAM_GAIN_KP,1) ) / 2;
	reference_speed( &nv, &nw );

	x = odm->x + nv * cos( odm->theta ) * delay;
	y = odm->y + nv * sin( odm->theta ) * delay;

	a = ( x - spur->x ) * cos( spur->theta ) + ( y - spur->y ) * sin( spur->theta );
	over = 0;

	vel = sqrt( 2 * spur->dv * fabs( a ) );

	if( fabs( spur->v ) < vel )
	{
		SpurUserParams spur_line;

		spur_line = *spur;
		spur_line.v = -SIGN( a ) * fabs( spur->v );

		line_follow( odm, &spur_line );
	}
	else
	{
		vel = -SIGN( a ) * vel;

		q = odm->theta - spur->theta;
		q = trans_q( q );

		if( fabs( a ) < p(YP_PARAM_STOP_LINEAR,0) )
		{
			vel = - sqrt( 2 * spur->dv * p(YP_PARAM_STOP_LINEAR,0) ) * a / p(YP_PARAM_STOP_LINEAR,0);
		}
		regurator( 0, q, 1000, vel, spur->w, spur );
	}

	if( a > 0.05 )
	{
		// 越えている
		over = 3;
	}
	else if( a < -0.05 )
	{
		// まだ
		over = 1;
		// vel = dist;
	}
	else
	{
		// 大体乗った
		over = 2;
		// vel = dist;
	}
	return over;
}


/* ホイール角度指令 */
double wheel_angle( OdometryPtr odm, SpurUserParamsPtr spur )
{
	double q, w_limit;
	double wr, wl;
	double nwl, nwr;
	double delay;

	reference_motor_speed( &nwl, &nwr );

	delay = spur->control_dt + ( 1 / p(YP_PARAM_GAIN_KP,MOTOR_LEFT) ) * 2;
	q = ( odm->theta_l + nwl * delay ) - spur->wheel_angle_l;
	/* 次の制御周期で停止するのに限界の速度を計算 */
	w_limit = sqrt( 2 * spur->wheel_accel_l * fabs( q ) );
	if( spur->wheel_vel_l < w_limit )
	{
		wl = -SIGN( q ) * fabs( spur->wheel_vel_l );
	}
	else
	{
		wl = -SIGN( q ) * w_limit;
	}
	if( fabs( q ) < p(YP_PARAM_WHEEL_ANG_LINEAR,MOTOR_LEFT) )
	{
		wl = -sqrt( 2 * spur->wheel_accel_l * p(YP_PARAM_WHEEL_ANG_LINEAR,MOTOR_LEFT) ) * q / p(YP_PARAM_WHEEL_ANG_LINEAR,MOTOR_LEFT);
	}
	if( wl > nwl + spur->wheel_accel_l * spur->control_dt )
	{
		wl = nwl + spur->wheel_accel_l * spur->control_dt;
	}
	else if( wl < nwl - spur->wheel_accel_l * spur->control_dt )
	{
		wl = nwl - spur->wheel_accel_l * spur->control_dt;
	}

	delay = spur->control_dt + ( 1 / p(YP_PARAM_GAIN_KP,MOTOR_RIGHT) ) * 2;
	q = ( odm->theta_r + nwr * delay ) - spur->wheel_angle_r;
	/* 次の制御周期で停止するのに限界の速度を計算 */
	w_limit = sqrt( 2 * spur->wheel_accel_r * fabs( q ) );
	if( spur->wheel_vel_r < w_limit )
	{
		wr = -SIGN( q ) * fabs( spur->wheel_vel_r );
	}
	else
	{
		wr = -SIGN( q ) * w_limit;
	}
	if( fabs( q ) < p(YP_PARAM_WHEEL_ANG_LINEAR,MOTOR_RIGHT) )
	{
		wr = -sqrt( 2 * spur->wheel_accel_r * p(YP_PARAM_WHEEL_ANG_LINEAR,MOTOR_RIGHT) ) * q / p(YP_PARAM_WHEEL_ANG_LINEAR,MOTOR_RIGHT);
	}
	if( wr > nwr + spur->wheel_accel_r * spur->control_dt )
	{
		wr = nwr + spur->wheel_accel_r * spur->control_dt;
	}
	else if( wr < nwr - spur->wheel_accel_r * spur->control_dt )
	{
		wr = nwr - spur->wheel_accel_r * spur->control_dt;
	}

	motor_speed( wr, wl );
	return 0;
}

/* ホイール速度指令 */
double wheel_vel( OdometryPtr odm, SpurUserParamsPtr spur )
{
	double wr, wl;
	double nwl, nwr;

	reference_motor_speed( &nwl, &nwr );

	wl = spur->wlref;
	if( wl > spur->wheel_vel_l ) wl = spur->wheel_vel_l;
	else if( wl < -spur->wheel_vel_l ) wl = -spur->wheel_vel_l;

	if( wl > nwl + spur->wheel_accel_l * spur->control_dt )
	{
		wl = nwl + spur->wheel_accel_l * spur->control_dt;
	}
	else if( wl < nwl - spur->wheel_accel_l * spur->control_dt )
	{
		wl = nwl - spur->wheel_accel_l * spur->control_dt;
	}

	wr = spur->wrref;
    if( wr > spur->wheel_vel_r ) wr = spur->wheel_vel_r;
    else if( wr < -spur->wheel_vel_r ) wr = -spur->wheel_vel_r;

	if( wr > nwr + spur->wheel_accel_r * spur->control_dt )
	{
		wr = nwr + spur->wheel_accel_r * spur->control_dt;
	}
	else if( wr < nwr - spur->wheel_accel_r * spur->control_dt )
	{
		wr = nwr - spur->wheel_accel_r * spur->control_dt;
	}

	motor_speed( wr, wl );
	return 0;
}


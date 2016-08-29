
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
	double v, w;
	double cd;
	double wref;


	v = v_max - SIGN( v_max ) * p( YP_PARAM_L_C1, 0 ) * fabs( spur->wref );
	if( v * v_max < 0 ) v = 0;

	wref = v / r;
	if( wref > fabs( w_max ) )
		wref = fabs( w_max );
	else if( wref < -fabs( w_max ) )
		wref = -fabs( w_max );

	cd = d;
	if( cd > p( YP_PARAM_L_DIST, 0 ) )
		cd = p( YP_PARAM_L_DIST, 0 );
	if( cd < -p( YP_PARAM_L_DIST, 0 ) )
		cd = -p( YP_PARAM_L_DIST, 0 );
	w = spur->wref_smooth -
		spur->control_dt * ( SIGN( r ) * SIGN( v_max ) * p( YP_PARAM_L_K1, 0 ) * cd +
							 p( YP_PARAM_L_K2, 0 ) * q + p( YP_PARAM_L_K3, 0 ) * ( spur->wref_smooth - wref ) );

	spur->vref = v;
	spur->wref = w;
	return d;
}

/* 回転 */
double spin( OdometryPtr odm, SpurUserParamsPtr spur )
{
	double w, theta;
	double dt;

	dt = p( YP_PARAM_CONTROL_CYCLE, 0 ) * 1.5;
	theta = odm->theta + spur->wref_smooth * dt;
	w = timeoptimal_servo( 
			trans_q( theta - spur->theta ),
			spur->w,
			0,
			spur->dw );

	spur->wref = w;
	spur->vref = 0;
	return fabs( odm->theta - spur->theta );
}

/* 方角 */
double orient( OdometryPtr odm, SpurUserParamsPtr spur )
{
	double w, theta;
	double dt;

	dt = p( YP_PARAM_CONTROL_CYCLE, 0 ) * 1.5;
	theta = odm->theta + spur->wref_smooth * dt;
	w = timeoptimal_servo( 
			trans_q( theta - spur->theta ),
			spur->w,
			0,
			spur->dw );

	spur->wref = w;
	spur->vref = spur->v;
	return fabs( odm->theta - spur->theta );
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
	double x, y;
	double dt;

	dt = p( YP_PARAM_CONTROL_CYCLE, 0 ) * 1.5;
	x = odm->x + spur->vref_smooth * cos( odm->theta ) * dt;
	y = odm->y + spur->vref_smooth * sin( odm->theta ) * dt;

	a = ( x - spur->x ) * cos( spur->theta ) + ( y - spur->y ) * sin( spur->theta );
	vel = timeoptimal_servo( 
			a,
			spur->v,
			0,
			spur->dv );
	over = 0;

	q = odm->theta - spur->theta;
	q = trans_q( q );
	regurator( 0, q, 1000, vel, spur->w, spur );

	if( a > 0.05 )
	{
		// 越えている
		over = 3;
	}
	else if( a < -0.05 )
	{
		// まだ
		over = 1;
	}
	else
	{
		// 大体乗った
		over = 2;
	}
	return over;
}

double timeoptimal_servo( double err, double vel_max, double vel, double acc )
{
	double vel_ref_next;
	double v;
	double _err;

	_err = err + vel * p( YP_PARAM_CONTROL_CYCLE, 0 ) * 1.5;
	if( _err * err < 0 ) _err = 0;

	// 次の目標位置で停止するために必要な現在の速度を計算
	v = sqrt( 2 * acc * fabs( _err ) );
	if( vel_max < v )
	{
		vel_ref_next = -SIGN( _err ) * fabs( vel_max );
	}
	else
	{
		vel_ref_next = -SIGN( _err ) * v;
	}

	// 次の制御周期で目標値をこえてしまう場合をクリップ
	if( ( err + vel_ref_next * p( YP_PARAM_CONTROL_CYCLE, 0 ) * 1.5 ) * ( err ) < 0 )
	{
		vel_ref_next = -err / p( YP_PARAM_CONTROL_CYCLE, 0 );
	}
	return vel_ref_next;
}

double timeoptimal_servo2( double err, double vel_max, double vel, double acc, double vel_end )
{
	double v;
	double _err;
	double _vel_max;

	_err = err + vel * p( YP_PARAM_CONTROL_CYCLE, 0 ) * 1.5;

	v = sqrt( vel_end * vel_end + 2 * acc * fabs( _err ) );

	if( fabs(vel_max) < fabs(vel_end) )
	{
		if( fabs( err ) < (vel_end * vel_end - vel_max * vel_max) / (2.0 * acc) )
			_vel_max = fabs(vel_end);
		else
			_vel_max = vel_max;
	}
	else
		_vel_max = vel_max;

	if( _vel_max < v )
	{
		v = _vel_max;
	}
	if( _err > 0 )
	{
		v = -v;
	}

	return v;
}
								
								
							




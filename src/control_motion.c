
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

	v = v_max - SIGN( v_max ) * p( YP_PARAM_L_C1 ) * fabs( nw );
	wref = v_max / r;
	if( wref > fabs( w_max ) )
		wref = fabs( w_max );
	else if( wref < -fabs( w_max ) )
		wref = -fabs( w_max );

	// printf("d%f q%f r%f v%f w%f wref%f\n",d,q,r,nv,nw,wref);fflush(stdout);
	cd = d;
	if( cd > p( YP_PARAM_L_DIST ) )
		cd = p( YP_PARAM_L_DIST );
	if( cd < -p( YP_PARAM_L_DIST ) )
		cd = -p( YP_PARAM_L_DIST );
	w = nw -
		spur->control_dt * ( SIGN( r ) * SIGN( v_max ) * p( YP_PARAM_L_K1 ) * cd +
							 p( YP_PARAM_L_K2 ) * q + p( YP_PARAM_L_K3 ) * ( nw - wref ) );
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

	reference_speed( &nv, &nw );

	q = ( odm->theta + nw * spur->control_dt ) - spur->theta;
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
	// double theta;

	// dist = dist_pos( odm, x, y );
	reference_speed( &nv, &nw );

	x = odm->x + nv * cos( odm->theta ) * spur->control_dt;
	y = odm->y + nv * sin( odm->theta ) * spur->control_dt;
	// theta = odm->theta + nw * spur->control_dt;

	a = ( x - spur->x ) * cos( spur->theta ) + ( y - spur->y ) * sin( spur->theta );
	// a *= SIGN( spur->v );
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

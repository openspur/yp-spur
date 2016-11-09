/* high level I/O */
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <strings.h>

/* low level I/O */
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>

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
#include <cartesian2d.h>

void get_pos_com( int cs, double *data, double *resdata, SpurUserParamsPtr spur )
{
	Odometry odometry;

	cstrans_odometry( cs, &odometry );

	resdata[0] = odometry.x;
	resdata[1] = odometry.y;
	resdata[2] = odometry.theta;
	resdata[3] = odometry.time;
	// printf( "get %f %f %f\n", x, y, theta );
}

void get_wheel_vref_com( int cs, double *data, double *resdata, SpurUserParamsPtr spur )
{
	OdometryPtr odometry;
	
	odometry = get_odometry_ptr(  );

	resdata[0] = spur->wvelref[0];
	resdata[1] = spur->wvelref[1];
	resdata[2] = odometry->time;
}

void get_vref_com( int cs, double *data, double *resdata, SpurUserParamsPtr spur )
{
	OdometryPtr odometry;
	
	odometry = get_odometry_ptr(  );

	resdata[0] = spur->vref_smooth;
	resdata[1] = spur->wref_smooth;
	resdata[2] = odometry->time;
}

void get_vel_com( int cs, double *data, double *resdata, SpurUserParamsPtr spur )
{
	OdometryPtr odometry;

	odometry = get_odometry_ptr(  );

	resdata[0] = odometry->v;
	resdata[1] = odometry->w;
	resdata[2] = odometry->time;

	// printf("getvel %f %f %f\n",);
}

void get_force_com( int cs, double *data, double *resdata, SpurUserParamsPtr spur )
{
	OdometryPtr odometry;

	odometry = get_odometry_ptr(  );

	resdata[0] = odometry->torque_trans;
	resdata[1] = odometry->torque_angular;
	resdata[2] = odometry->time;

	// printf("getvel %f %f %f\n",);
}

void get_wheel_torque_com( int cs, double *data, double *resdata, SpurUserParamsPtr spur )
{
	OdometryPtr odometry;

	odometry = get_odometry_ptr(  );

	resdata[0] = odometry->wtorque[0];
	resdata[1] = odometry->wtorque[1];
	resdata[2] = odometry->time;

	// printf("getvel %f %f %f\n",);
}

void get_wheel_vel_com( double *data, double *resdata, SpurUserParamsPtr spur )
{
	OdometryPtr odometry;

	odometry = get_odometry_ptr(  );

	resdata[0] = odometry->wvel[0];
	resdata[1] = odometry->wvel[1];
	resdata[2] = odometry->time;

	// printf("getvel %f %f %f\n",);
}

void get_wheel_ang_com( double *data, double *resdata, SpurUserParamsPtr spur )
{
	OdometryPtr odometry;

	odometry = get_odometry_ptr(  );

	resdata[0] = odometry->wang[0];
	resdata[1] = odometry->wang[1];
	resdata[2] = odometry->time;

	// printf("getvel %f %f %f\n",);
}

int near_pos_com( int cs, double *data, double *resdata, SpurUserParamsPtr spur )
{
	double x, y, theta, cx, cy;
	double dist;
	OdometryPtr odometry;

	odometry = get_odometry_ptr(  );

	x = odometry->x;
	y = odometry->y;
	theta = odometry->theta;

	cstrans_xy( CS_BS, cs, &x, &y, &theta );

	cx = data[0];
	cy = data[1];
	dist = sqrt( ( cx - x ) * ( cx - x ) + ( cy - y ) * ( cy - y ) );
	resdata[0] = dist;

	if( dist < data[2] )
		return 1;
	else
		return 0;
}

int near_ang_com( int cs, double *data, double *resdata, SpurUserParamsPtr spur )
{
	double x, y, theta;
	double dist;
	OdometryPtr odometry;

	odometry = get_odometry_ptr(  );

	x = odometry->x;
	y = odometry->y;
	theta = odometry->theta;

	cstrans_xy( CS_BS, cs, &x, &y, &theta );

	dist = theta - data[0];
	while( dist > M_PI )
		dist -= 2 * M_PI;
	while( dist < -M_PI )
		dist += 2 * M_PI;
	resdata[0] = dist;
	if( fabs( dist ) < data[1] )
		return 1;
	else
		return 0;
}

int over_line_com( int cs, double *data, double *resdata, SpurUserParamsPtr spur )
{
	double x, y, theta;
	double dist;
	OdometryPtr odometry;

	odometry = get_odometry_ptr(  );

	x = odometry->x;
	y = odometry->y;
	theta = odometry->theta;

	cstrans_xy( CS_BS, cs, &x, &y, &theta );

	dist = ( x - data[0] ) * cos( data[2] ) + ( y - data[1] ) * sin( data[2] );

	resdata[0] = dist;
	if( dist > 0 )
		return 1;
	else
		return 0;
}

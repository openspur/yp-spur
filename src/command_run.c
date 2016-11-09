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

/* SSM 用 */
// #include <ssm.h>
// #include <ssmtype/spur-odometry.h>

/* yp-spur用 */
#include <serial.h>
#include <param.h>
#include <control.h>
#include <command.h>

/* ライブラリ用 */
#include <ypspur.h>
#include <cartesian2d.h>

void line_com( int cs, double *data, SpurUserParamsPtr spur )
{
	double x, y, theta;

	x = data[0];
	y = data[1];
	theta = data[2];

	cstrans_xy( cs, CS_SP, &x, &y, &theta );

	spur->x = x;
	spur->y = y;
	spur->theta = theta;
	spur->run_mode = RUN_LINEFOLLOW;
}

void stop_line_com( int cs, double *data, SpurUserParamsPtr spur )
{
	double x, y, theta;

	x = data[0];
	y = data[1];
	theta = data[2];

	cstrans_xy( cs, CS_SP, &x, &y, &theta );

	spur->x = x;
	spur->y = y;
	spur->theta = theta;
	spur->run_mode = RUN_STOP_LINE;
}

void circle_com( int cs, double *data, SpurUserParamsPtr spur )
{
	double x, y, theta;

	x = data[0];
	y = data[1];
	theta = 0;

	cstrans_xy( cs, CS_SP, &x, &y, &theta );

	spur->x = x;
	spur->y = y;
	spur->radius = data[2];
	spur->run_mode = RUN_CIRCLEFOLLOW;
}

void spin_com( int cs, double *data, SpurUserParamsPtr spur )
{
	double x, y, theta;

	x = 0;
	y = 0;
	theta = data[0];

	cstrans_xy( cs, CS_SP, &x, &y, &theta );

	spur->theta = theta;
	spur->run_mode = RUN_SPIN;
}

void orient_com( int cs, double *data, SpurUserParamsPtr spur )
{
	double x, y, theta;

	x = 0;
	y = 0;
	theta = data[0];

	cstrans_xy( cs, CS_SP, &x, &y, &theta );

	spur->theta = theta;
	spur->run_mode = RUN_ORIENT;
}

void stop_com( double *data, SpurUserParamsPtr spur )
{
	spur->run_mode = RUN_STOP;
}

void free_com( double *data, SpurUserParamsPtr spur )
{
	spur->run_mode = RUN_FREE;
}

void openfree_com( double *data, SpurUserParamsPtr spur )
{
	spur->run_mode = RUN_OPENFREE;
}

void vel_com( double *data, SpurUserParamsPtr spur )
{
	spur->vref = data[0];
	spur->wref = data[1];
	spur->run_mode = RUN_VEL;
}

void wheel_vel_com( double *data, SpurUserParamsPtr spur )
{
	spur->wvelref[0] = data[0];
	spur->wvelref[1] = data[1];
	spur->run_mode = RUN_WHEEL_VEL;
}

void wheel_angle_com( double *data, SpurUserParamsPtr spur )
{
	double r, l;
	
	r= data[0];
	l= data[1];

	if( isset_p( YP_PARAM_MIN_WHEEL_ANGLE, MOTOR_RIGHT ) )
	{
		if( r < p( YP_PARAM_MIN_WHEEL_ANGLE, MOTOR_RIGHT ) ) r = p( YP_PARAM_MIN_WHEEL_ANGLE, MOTOR_RIGHT );
	}
	if( isset_p( YP_PARAM_MIN_WHEEL_ANGLE, MOTOR_LEFT ) )
	{
		if( l < p( YP_PARAM_MIN_WHEEL_ANGLE, MOTOR_LEFT ) ) l = p( YP_PARAM_MIN_WHEEL_ANGLE, MOTOR_LEFT );
	}
	if( isset_p( YP_PARAM_MAX_WHEEL_ANGLE, MOTOR_RIGHT ) )
	{
		if( r > p( YP_PARAM_MAX_WHEEL_ANGLE, MOTOR_RIGHT ) ) r = p( YP_PARAM_MAX_WHEEL_ANGLE, MOTOR_RIGHT );
	}
	if( isset_p( YP_PARAM_MAX_WHEEL_ANGLE, MOTOR_LEFT ) )
	{
		if( l > p( YP_PARAM_MAX_WHEEL_ANGLE, MOTOR_LEFT ) ) l = p( YP_PARAM_MAX_WHEEL_ANGLE, MOTOR_LEFT );
	}
	
	spur->wheel_angle[1] = l;
	spur->wheel_angle[0] = r;
	spur->run_mode = RUN_WHEEL_ANGLE;
}


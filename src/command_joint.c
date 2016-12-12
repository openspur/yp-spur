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
#include <param.h>
#include <control.h>
#include <command.h>
#include <yprintf.h>

/* ライブラリ用 */
#include <ypspur.h>

void joint_torque_com( int id, double *data, SpurUserParamsPtr spur )
{
	if( id > YP_PARAM_MAX_MOTOR_NUM )
	{
		yprintf(OUTPUT_LV_ERROR, "Motor id out of range (%d)\n", id);
		return;
	}
	spur->torque[id] = data[0];
	spur->wheel_mode[id] = MOTOR_CONTROL_FREE;
}

void joint_vel_com( int id, double *data, SpurUserParamsPtr spur )
{
	if( id > YP_PARAM_MAX_MOTOR_NUM )
	{
		yprintf(OUTPUT_LV_ERROR, "Motor id out of range (%d)\n", id);
		return;
	}
	spur->wvelref[id] = data[0];
	spur->wheel_mode[id] = MOTOR_CONTROL_VEL;
}

void joint_ang_com( int id, double *data, SpurUserParamsPtr spur )
{
	if( id > YP_PARAM_MAX_MOTOR_NUM )
	{
		yprintf(OUTPUT_LV_ERROR, "Motor id out of range (%d)\n", id);
		return;
	}
	spur->wheel_angle[id] = data[0];
	spur->wheel_mode[id] = MOTOR_CONTROL_ANGLE;
}

void joint_ang_vel_com( int id, double *data, SpurUserParamsPtr spur )
{
	if( id > YP_PARAM_MAX_MOTOR_NUM )
	{
		yprintf(OUTPUT_LV_ERROR, "Motor id out of range (%d)\n", id);
		return;
	}
	spur->wheel_angle[id] = data[0];
	spur->wheel_vel_end[id] = data[1];
	spur->wheel_mode[id] = MOTOR_CONTROL_ANGLE_VEL;
}

void set_joint_accel_com( int id, double *data, SpurUserParamsPtr spur )
{
	if( id > YP_PARAM_MAX_MOTOR_NUM )
	{
		yprintf(OUTPUT_LV_ERROR, "Motor id out of range (%d)\n", id);
		return;
	}
	spur->wheel_accel[id] = data[0];
}

void set_joint_vel_com( int id, double *data, SpurUserParamsPtr spur )
{
	if( id > YP_PARAM_MAX_MOTOR_NUM )
	{
		yprintf(OUTPUT_LV_ERROR, "Motor id out of range (%d)\n", id);
		return;
	}
	spur->wheel_vel[id] = data[0];
}

void get_joint_vel_com( int id, double *data, SpurUserParamsPtr spur )
{
	OdometryPtr odometry;
	
	if( id > YP_PARAM_MAX_MOTOR_NUM )
	{
		yprintf(OUTPUT_LV_ERROR, "Motor id out of range (%d)\n", id);
		return;
	}
	odometry = get_odometry_ptr(  );

	data[0] = odometry->wvel[id];
	data[1] = odometry->time;
}

void get_joint_vref_com( int id, double *data, SpurUserParamsPtr spur )
{
	OdometryPtr odometry;
	
	if( id > YP_PARAM_MAX_MOTOR_NUM )
	{
		yprintf(OUTPUT_LV_ERROR, "Motor id out of range (%d)\n", id);
		return;
	}
	odometry = get_odometry_ptr(  );

	data[0] = spur->wvelref[id];
	data[1] = odometry->time;
}

void get_joint_ang_com( int id, double *data, SpurUserParamsPtr spur )
{
	OdometryPtr odometry;
	
	if( id > YP_PARAM_MAX_MOTOR_NUM )
	{
		yprintf(OUTPUT_LV_ERROR, "Motor id out of range (%d)\n", id);
		return;
	}
	odometry = get_odometry_ptr(  );

	data[0] = odometry->wang[id];
	data[1] = odometry->time;
}

void get_joint_torque_com( int id, double *data, SpurUserParamsPtr spur )
{
	OdometryPtr odometry;
	
	if( id > YP_PARAM_MAX_MOTOR_NUM )
	{
		yprintf(OUTPUT_LV_ERROR, "Motor id out of range (%d)\n", id);
		return;
	}
	odometry = get_odometry_ptr(  );

	data[0] = odometry->wtorque[id];
	data[1] = odometry->time;
}



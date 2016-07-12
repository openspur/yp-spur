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
#include <command.h>
#include <odometry.h>
#include <control.h>
#include <utility.h>
#include <yprintf.h>

/* ライブラリ用 */
#include <ypspur.h>
#include <cartesian2d.h>

void param_set_com( int cs, double *data, SpurUserParamsPtr spur )
{
	if( cs >= 0 && cs < YP_PARAM_NUM )
	{
		*pp( cs, 0 ) = data[0];
		*pp( cs, 1 ) = data[1];
	}
}

int param_get_com( int cs, double *resdata, SpurUserParamsPtr spur )
{
	if( cs >= 0 && cs < YP_PARAM_NUM )
	{
		resdata[0] = *pp( cs, 0 );
		resdata[1] = *pp( cs, 1 );
		return cs;
	}
	else
	{
		return 0;
	}
}

void param_set_motor_com( int cs, double *data, SpurUserParamsPtr spur )
{
	if( cs >= 0 && cs < YP_PARAM_NUM )
		*pp( cs, data[0] ) = data[1];
}

int param_get_motor_com( int cs, double *resdata, SpurUserParamsPtr spur )
{
	if( cs >= 0 && cs < YP_PARAM_NUM )
	{
		resdata[0] = *pp( cs, resdata[0] );
		return cs;
	}
	else
	{
		return 0;
	}
}

void param_state_com( int cs, double *data, SpurUserParamsPtr spur )
{
	OdometryPtr odometry;

	odometry = get_odometry_ptr(  );

	if( cs >= 0 && cs < YP_STATE_NUM )
	{
		if( data[0] )
		{
			enable_state( cs );
		}
		else
		{
			disable_state( cs );
		}
	}

	if( cs == YP_STATE_MOTOR && data[0] == ENABLE )
	{
		set_param_motor(  );
		yprintf( OUTPUT_LV_CONTROL, "COMMAND: Motor Control: enabled\n" );
	}
	if( cs == YP_STATE_VELOCITY && data[0] == ENABLE )
	{
		set_param_velocity(  );
		yprintf( OUTPUT_LV_CONTROL, "COMMAND: Velocity Control: enabled\n" );
	}
	if( cs == YP_STATE_BODY && data[0] == ENABLE )
	{
		spur->vref_smooth = 0;
		spur->wref_smooth = 0;
		robot_speed( spur );
		odometry->x = 0;
		odometry->y = 0;
		odometry->theta = 0;
		yprintf( OUTPUT_LV_CONTROL, "COMMAND: Body Control: enabled\n" );
	}
	if( cs == YP_STATE_TRACKING && data[0] == ENABLE )
	{
		yprintf( OUTPUT_LV_CONTROL, "COMMAND: Trajectory Control: enabled\n" );
	}
	if( cs == YP_STATE_GRAVITY && data[0] == ENABLE )
	{
		yprintf( OUTPUT_LV_CONTROL, "COMMAND: Gravity Compensation: enabled\n" );
	}

}

/* high level I/O */
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <strings.h>
#include <pthread.h>

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

/* ライブラリ用 */
#include <ypspur.h>
#include <cartesian2d.h>

void param_set_com( int cs, double *data, SpurUserParamsPtr spur )
{
	if( cs >= 0 && cs < YP_PARAM_NUM )
		*pp( cs ) = data[0];
}

int param_get_com( int cs, double *resdata, SpurUserParamsPtr spur )
{
	if( cs >= 0 && cs < YP_PARAM_NUM )
	{
		resdata[0] = *pp( cs );
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
		if( output_lv(  ) >= OUTPUT_LV_CONTROL )
			printf( "COMMAND: Motor Control: enabled\n" );
	}
	if( cs == YP_STATE_VELOCITY && data[0] == ENABLE )
	{
		set_param_velocity(  );
		if( output_lv(  ) >= OUTPUT_LV_CONTROL )
			printf( "COMMAND: Velocity Control: enabled\n" );
	}
	if( cs == YP_STATE_BODY && data[0] == ENABLE )
	{
		robot_speed( 0, 0 );
		odometry->x = 0;
		odometry->y = 0;
		odometry->theta = 0;
		if( output_lv(  ) >= OUTPUT_LV_CONTROL )
			printf( "COMMAND: Body Control: enabled\n" );
	}
	if( cs == YP_STATE_TRACKING && data[0] == ENABLE )
	{
		if( output_lv(  ) >= OUTPUT_LV_CONTROL )
			printf( "COMMAND: Trajectory Control: enabled\n" );
	}
	if( cs == YP_STATE_GRAVITY && data[0] == ENABLE )
	{

		if( output_lv(  ) >= OUTPUT_LV_CONTROL )
			printf( "COMMAND: Gravity Compensation: enabled\n" );
	}

}

/* 
 * 
 * 
 */
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include <ssm.h>
#include <ssmtype/spur-odometry.h>
#include <odometry-ssm.h>

static SSM_sid odm_bs_sid;
static SSM_sid odm_gl_sid;

/* poetとのメッセージ通信を開始する */
int OdometrySSM_init( void )
{
	/* オープン */
	if( !initSSM(  ) )
		return 0;

	odm_bs_sid = openSSM( "spur_odometry", 0, 0 );
	odm_gl_sid = openSSM( "spur_global", 0, 0 );
	return 1;
}

/* 線形補間で位置取得 */
int SSM_get_pos_GL_time( double time, double *x, double *y, double *theta )
{
	Spur_Odometry odm1, odm2;
	int tid, tid2;
	double time1, time2;
	double rate;

	tid = readSSM_time( odm_gl_sid, ( char * )&odm1, time, &time1 );
	tid2 = readSSM( odm_gl_sid, ( char * )&odm2, &time2, tid + 1 );
	if( tid2 < 0 )
	{											/* 次のがなければ前のでなんとか。 */
		time2 = time1;
		odm2 = odm1;
		tid = readSSM( odm_gl_sid, ( char * )&odm1, &time1, tid - 1 );
	}
	if( fabs( time2 - time1 ) < 0.000001 )
		return 0;
	if( fabs( time2 - time1 ) > 0.03 )
		return 0;

	// printf("%f %f %f\n",time-time1,time2-time, time2-time1);

	rate = ( time - time1 ) / ( time2 - time1 );
	*x = odm1.x + ( odm2.x - odm1.x ) * rate;
	*y = odm1.y + ( odm2.y - odm1.y ) * rate;
	while( odm2.theta - odm1.theta < -M_PI )
		odm2.theta += 2.0 * M_PI;
	while( odm2.theta - odm1.theta > M_PI )
		odm2.theta -= 2.0 * M_PI;
	*theta = odm1.theta + ( odm2.theta - odm1.theta ) * rate;
	while( odm2.theta - odm1.theta < -M_PI )
		odm2.theta += 2.0 * M_PI;
	while( odm2.theta - odm1.theta > M_PI )
		odm2.theta -= 2.0 * M_PI;

	return 1;
}

/* 線形補間で位置取得 */
int SSM_get_pos_BS_time( double time, double *x, double *y, double *theta )
{
	Spur_Odometry odm1, odm2;
	int tid, tid2;
	double time1, time2;
	double rate;

	tid = readSSM_time( odm_bs_sid, ( char * )&odm1, time, &time1 );
	tid2 = readSSM( odm_bs_sid, ( char * )&odm2, &time2, tid + 1 );
	if( tid2 < 0 )
	{											/* 次のがなければ前のでなんとか。 */
		time2 = time1;
		odm2 = odm1;
		tid = readSSM( odm_bs_sid, ( char * )&odm1, &time1, tid - 1 );
	}
	if( fabs( time2 - time1 ) < 0.000001 )
		return 0;
	if( fabs( time2 - time1 ) > 0.03 )
		return 0;

	// printf("%f %f %f\n",time-time1,time2-time, time2-time1);

	rate = ( time - time1 ) / ( time2 - time1 );
	*x = odm1.x + ( odm2.x - odm1.x ) * rate;
	*y = odm1.y + ( odm2.y - odm1.y ) * rate;
	while( odm2.theta - odm1.theta < -M_PI )
		odm2.theta += 2.0 * M_PI;
	while( odm2.theta - odm1.theta > M_PI )
		odm2.theta -= 2.0 * M_PI;
	*theta = odm1.theta + ( odm2.theta - odm1.theta ) * rate;
	while( odm2.theta - odm1.theta < -M_PI )
		odm2.theta += 2.0 * M_PI;
	while( odm2.theta - odm1.theta > M_PI )
		odm2.theta -= 2.0 * M_PI;

	return 1;
}

/* */
double SSM_get_pos_GL( double *x, double *y, double *theta )
{
	Spur_Odometry odm1;
	int tid;
	double time1;

	tid = readSSM( odm_gl_sid, ( char * )&odm1, &time1, -1 );
	if( tid < 0 )
		return -1.0;
	*x = odm1.x;
	*y = odm1.y;
	*theta = odm1.theta;

	return time1;
}

/* */
double SSM_get_pos_BS( double *x, double *y, double *theta )
{
	Spur_Odometry odm1;
	int tid;
	double time1;

	tid = readSSM( odm_bs_sid, ( char * )&odm1, &time1, -1 );
	if( tid < 0 )
		return -1.0;

	*x = odm1.x;
	*y = odm1.y;
	*theta = odm1.theta;

	return time1;
}

/* */
double SSM_get_vel( double *v, double *w )
{

	Spur_Odometry odm1;
	int tid;
	double time1;

	tid = readSSM( odm_bs_sid, ( char * )&odm1, &time1, -1 );
	if( tid < 0 )
		return -1.0;


	return time1;
}

int SSM_near_pos_GL( double x, double y, double r )
{
	double cx, cy, ctheta;
	double dist;

	SSM_get_pos_GL( &cx, &cy, &ctheta );
	dist = sqrt( ( cx - x ) * ( cx - x ) + ( cy - y ) * ( cy - y ) );
	if( dist < r )
		return 1;
	else
		return 0;
}

int SSM_near_ang( double th, double d )
{
	double cx, cy, ctheta;
	double dist;

	SSM_get_pos_GL( &cx, &cy, &ctheta );
	dist = ctheta - th;
	while( dist < -M_PI )
		dist += 2 * M_PI;
	while( dist > M_PI )
		dist -= 2 * M_PI;
	if( fabs( dist ) < d )
		return 1;
	else
		return 0;
}

int SSM_over_line( double x, double y, double theta )
{
	return 0;
}

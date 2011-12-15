#ifndef __ODOMETRY_SSM__
#define __ODOMETRY_SSM__

#ifdef __cplusplus
extern "C"
{
#endif

	int OdometrySSM_init( void );
	int SSM_get_pos_GL_time( double time, double *x, double *y, double *theta );
	int SSM_get_pos_BS_time( double time, double *x, double *y, double *theta );
	double SSM_get_pos_GL( double *x, double *y, double *theta );
	double SSM_get_pos_BS( double *x, double *y, double *theta );
	double SSM_get_vel( double *v, double *w );
	int SSM_near_pos_GL( double x, double y, double r );
	int SSM_near_ang( double th, double d );
	int SSM_over_line( double x, double y, double theta );

#ifdef __cplusplus
}
#endif
#endif

#ifndef __ODOMETRY__
#define __ODOMETRY__

#include <cartesian2d.h>
#include <ypspur.h>

typedef struct _odometry *OdometryPtr;
typedef struct _odometry
{
	double x;
	double y;
	double theta;
	double v;
	double w;
	double time;
	double wr;
	double wl;
	double theta_r;
	double theta_l;
	double torque_r;
	double torque_l;
	double torque_trans;
	double torque_angular;
} Odometry;

double time_estimate( int readnum );
void cstrans_odometry( YPSpur_cs cs, OdometryPtr dst_odm );
void cstrans_xy( YPSpur_cs src, YPSpur_cs dest, double *x, double *y, double *theta );
void odometry( OdometryPtr xp, short cnt1, short cnt2, short pwm1, short pwm2, double dt );
void odm_logging( OdometryPtr, double, double );
int odm_read( OdometryPtr odm, double *v, double *w );
void cs_odometry( YPSpur_cs cs, OdometryPtr dst_odm );
void init_odometry( void );
void init_coordinate_systems( void );
int odometry_receive_loop( void );
OdometryPtr get_odometry_ptr(  );
CSptr get_cs_pointer( YPSpur_cs cs );
void set_cs( YPSpur_cs cs, double x, double y, double theta );

#endif

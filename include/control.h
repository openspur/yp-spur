#ifndef __CONTROL__
#define __CONTROL__

#include <command.h>
#include <odometry.h>

int robot_speed_smooth( double v, double w, SpurUserParamsPtr spur );
int motor_speed_smooth( double r, double l, SpurUserParamsPtr spur );
void robot_speed( double v, double w );
void motor_speed( double r, double l );

int reference_speed( double *v, double *w );
void update_ref_speed( );
int reference_motor_speed( double *wr, double *wl );

double timeoptimal_servo( double err, double vel_max, double vel, double acc );

double dist_pos( OdometryPtr odm, SpurUserParamsPtr spur );
double trans_q( double theta );

double circle_follow( OdometryPtr odm, SpurUserParamsPtr spur );
double line_follow( OdometryPtr odm, SpurUserParamsPtr spur );
int stop_line( OdometryPtr odm, SpurUserParamsPtr spur );
double spin( OdometryPtr odm, SpurUserParamsPtr spur );
double orient( OdometryPtr odm, SpurUserParamsPtr spur );
double wheel_angle( OdometryPtr odm, SpurUserParamsPtr spur );

double regurator( double d, double q, double r, double v_max, double w_max, SpurUserParamsPtr spur );

double gravity_compensation( OdometryPtr odm, SpurUserParamsPtr spur );
void run_control( Odometry odometry, SpurUserParamsPtr spur );
void control_loop( void );

void init_control_thread( pthread_t * thread );

void set_run_mode( SpurRunMode mode );
SpurRunMode get_run_mode( void );


#endif

#ifndef __YP_SPUR_H__
#define __YP_SPUR_H__

#include <ypparam.h>

#ifdef __cplusplus
extern "C"
{
#endif

	/* YPSpurコマンド集 */
#define Spur_init()               YPSpur_init()
#define Spur_initex(msq)          YPSpur_initex(msq)
#define Spur_init_socket(ip, port) YPSpur_init_socket(ip, port)
#define Spur_stop()               YPSpur_stop()
#define Spur_free()               YPSpur_free()

#define Spur_freeze()             YPSpur_freeze()
#define Spur_isfreeze()           YPSpur_isfreeze()
#define Spur_unfreeze()           YPSpur_unfreeze()

#define Spur_line_GL(x,y,th)    YPSpur_line(CS_GL, x,y,th)
#define Spur_line_LC(x,y,th)    YPSpur_line(CS_LC, x,y,th)
#define Spur_line_FS(x,y,th)    YPSpur_line(CS_FS, x,y,th)
#define Spur_line_BS(x,y,th)    YPSpur_line(CS_BS, x,y,th)
#define Spur_line_BL(x,y,th)    YPSpur_line(CS_BL, x,y,th)

#define Spur_stop_line_GL(x,y,th)    YPSpur_stop_line(CS_GL, x,y,th)
#define Spur_stop_line_LC(x,y,th)    YPSpur_stop_line(CS_LC, x,y,th)
#define Spur_stop_line_FS(x,y,th)    YPSpur_stop_line(CS_FS, x,y,th)
#define Spur_stop_line_BS(x,y,th)    YPSpur_stop_line(CS_BS, x,y,th)
#define Spur_stop_line_BL(x,y,th)    YPSpur_stop_line(CS_BL, x,y,th)

#define Spur_circle_GL(x,y,d)    YPSpur_circle(CS_GL, x,y,d)
#define Spur_circle_LC(x,y,d)    YPSpur_circle(CS_LC, x,y,d)
#define Spur_circle_FS(x,y,d)    YPSpur_circle(CS_FS, x,y,d)
#define Spur_circle_BS(x,y,d)    YPSpur_circle(CS_BS, x,y,d)
#define Spur_circle_BL(x,y,d)    YPSpur_circle(CS_BL, x,y,d)

#define Spur_spin_GL(th)    YPSpur_spin(CS_GL, th)
#define Spur_spin_LC(th)    YPSpur_spin(CS_LC, th)
#define Spur_spin_FS(th)    YPSpur_spin(CS_FS, th)
#define Spur_spin_BS(th)    YPSpur_spin(CS_BS, th)
#define Spur_spin_BL(th)    YPSpur_spin(CS_BL, th)

#define Spur_orient_GL(th)    YPSpur_orient(CS_GL, th)
#define Spur_orient_LC(th)    YPSpur_orient(CS_LC, th)
#define Spur_orient_FS(th)    YPSpur_orient(CS_FS, th)
#define Spur_orient_BS(th)    YPSpur_orient(CS_BS, th)
#define Spur_orient_BL(th)    YPSpur_orient(CS_BL, th)

#define Spur_set_pos_GL(x,y,th)    YPSpur_set_pos(CS_GL, x,y,th)
#define Spur_set_pos_LC(x,y,th)    YPSpur_set_pos(CS_LC, x,y,th)
#define Spur_set_pos_BL(x,y,th)    YPSpur_set_pos(CS_BL, x,y,th)

#define Spur_set_vel(v)      YPSpur_set_vel(v)
#define Spur_set_angvel(w)   YPSpur_set_angvel(w)
#define Spur_set_accel(v)    YPSpur_set_accel(v)
#define Spur_set_angaccel(w) YPSpur_set_angaccel(w)

#define Spur_adjust_pos_GL(x,y,th)    YPSpur_adjust_pos(CS_GL, x,y,th)
#define Spur_adjust_pos_LC(x,y,th)    YPSpur_adjust_pos(CS_LC, x,y,th)
#define Spur_adjust_pos_FS(x,y,th)    YPSpur_adjust_pos(CS_FS, x,y,th)
#define Spur_adjust_pos_BS(x,y,th)    YPSpur_adjust_pos(CS_BS, x,y,th)
#define Spur_adjust_pos_BL(x,y,th)    YPSpur_adjust_pos(CS_BL, x,y,th)

#define Spur_get_pos_GL(x,y,th)    YPSpur_get_pos(CS_GL, x,y,th)
#define Spur_get_pos_LC(x,y,th)    YPSpur_get_pos(CS_LC, x,y,th)
#define Spur_get_pos_BS(x,y,th)    YPSpur_get_pos(CS_BS, x,y,th)
#define Spur_get_pos_BL(x,y,th)    YPSpur_get_pos(CS_BL, x,y,th)

#define Spur_get_vel(v, w)              YPSpur_get_vel(v, w)
#define Spur_get_force(trans, angular)  YPSpur_get_force(trans, angular)

#define Spur_near_pos_GL(x,y,r)   YPSpur_near_pos(CS_GL,x,y,r)
#define Spur_near_pos_LC(x,y,r)   YPSpur_near_pos(CS_LC,x,y,r)
#define Spur_near_pos_BS(x,y,r)   YPSpur_near_pos(CS_BS,x,y,r)
#define Spur_near_pos_BL(x,y,r)   YPSpur_near_pos(CS_BL,x,y,r)

#define Spur_near_ang_GL(th,d)   YPSpur_near_ang(CS_GL,th,d)
#define Spur_near_ang_LC(th,d)   YPSpur_near_ang(CS_LC,th,d)
#define Spur_near_ang_BS(th,d)   YPSpur_near_ang(CS_BS,th,d)
#define Spur_near_ang_BL(th,d)   YPSpur_near_ang(CS_BL,th,d)

#define Spur_over_line_GL(x,y,th)   YPSpur_over_line(CS_GL,x,y,th)
#define Spur_over_line_LC(x,y,th)   YPSpur_over_line(CS_LC,x,y,th)
#define Spur_over_line_BS(x,y,th)   YPSpur_over_line(CS_BS,x,y,th)
#define Spur_over_line_BL(x,y,th)   YPSpur_over_line(CS_BL,x,y,th)

#define Spur_vel(v, w)             YPSpur_vel(v, w)

#define Spur_tilt_GL(d,t)          YPSpur_tilt(CS_GL,d,t)
#define Spur_tilt_LC(d,t)          YPSpur_tilt(CS_LC,d,t)
#define Spur_tilt_FS(d,t)          YPSpur_tilt(CS_FS,d,t)
#define Spur_tilt_BS(d,t)          YPSpur_tilt(CS_BS,d,t)
#define Spur_tilt_BL(d,t)          YPSpur_tilt(CS_BL,d,t)



	/** init */
	int YPSpur_init( void );
	int YPSpur_initex( int msq_key );
	int YPSpur_init_socket( char *ip, int port );

	/** freeze */
	int YPSpur_isfreeze( void );
	int YPSpur_freeze( void );
	int YPSpur_unfreeze( void );

	/** motor servo */
	int YPSpur_stop( void );
	int YPSpur_free( void );
	int YP_openfree( void );

	/** spur */
	int YPSpur_line( int cs, double x, double y, double theta );
	int YPSpur_stop_line( int cs, double x, double y, double theta );
	int YPSpur_circle( int cs, double x, double y, double r );
	int YPSpur_spin( int cs, double theta );
	int YPSpur_orient( int cs, double theta );

	/** setter */
	int YPSpur_set_pos( int cs, double x, double y, double theta );
	int YPSpur_adjust_pos( int cs, double x, double y, double theta );
	int YPSpur_set_vel( double v );
	int YPSpur_set_angvel( double w );
	int YPSpur_set_accel( double v );
	int YPSpur_set_angaccel( double w );

	/** getter */
	double YPSpur_get_pos( int cs, double *x, double *y, double *theta );
	double YPSpur_get_vel( double *v, double *w );
	double YPSpur_get_force( double *trans, double *angular );

	/** check position */
	int YPSpur_near_pos( int cs, double x, double y, double r );
	int YPSpur_near_ang( int cs, double th, double d );
	int YPSpur_over_line( int cs, double x, double y, double theta );

	/** direct */
	int YPSpur_vel( double v, double w );

	/** tilt */
	int YPSpur_tilt( int cs, double dir, double tilt );

	/* 裏コマンド集 */
	int YP_get_error_state( void );

	int YP_set_parameter( int param_id, double value );
	int YP_set_parameter_array( int param_id, double *value );
	int YP_get_parameter( int param_id, double *value );
	int YP_get_parameter_array( int param_id, double *value );
	int YP_set_control_state( int control_id, int state );

	int YP_get_ad_value( int num );
	int YP_set_io_dir( unsigned char dir );
	int YP_set_io_data( unsigned char data );
	int YP_wheel_vel( double r, double l );
	int YP_wheel_torque( double r, double l );
	double YP_get_wheel_vel( double *wr, double *wl );
	double YP_get_wheel_ang( double *theta_r, double *theta_l );
	double YP_get_wheel_torque( double *torque_r, double *torque_l );
	int YP_wheel_ang( double r, double l );
	int YP_set_wheel_accel( double r, double l );
	int YP_set_wheel_vel( double r, double l );
	double YP_get_vref( double *vref, double *wref );
	double YP_get_wheel_vref( double *wrref, double *wlref );

	int YP_joint_torque( int id, double t );
	int YP_joint_vel( int id, double v );
	int YP_joint_ang( int id, double a );
	int YP_joint_ang_vel( int id, double a, double v );
	int YP_set_joint_accel( int id, double a );
	int YP_set_joint_vel( int id, double v );
	double YP_get_joint_vel( int id, double *v );
	double YP_get_joint_vref( int id, double *v );
	double YP_get_joint_ang( int id, double *a );
	double YP_get_joint_torque( int id, double *t );


#define YPSPUR_JOINT_SUPPORT 1
#define YPSPUR_JOINT_ANG_VEL_SUPPORT 1

#ifdef __cplusplus
}
#endif
#endif

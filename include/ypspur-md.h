// Copyright (c) 2010-2016 The YP-Spur Authors, except where otherwise indicated.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef YPSPUR_MD_H
#define YPSPUR_MD_H

#include <ypparam.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#define Spur_md_init(dev) YPSpur_md_init(dev)
#define Spur_md_initex(dev, msq) YPSpur_md_initex(dev, msq)
#define Spur_md_init_socket(dev, ip, port) YPSpur_md_init_socket(dev, ip, port)
#define Spur_md_stop(dev) YPSpur_md_stop(dev)
#define Spur_md_free(dev) YPSpur_md_free(dev)

#define Spur_md_freeze(dev) YPSpur_md_freeze(dev)
#define Spur_md_isfreeze(dev) YPSpur_md_isfreeze(dev)
#define Spur_md_unfreeze(dev) YPSpur_md_unfreeze(dev)

#define Spur_md_line_GL(dev, x, y, th) YPSpur_md_line(dev, CS_GL, x, y, th)
#define Spur_md_line_LC(dev, x, y, th) YPSpur_md_line(dev, CS_LC, x, y, th)
#define Spur_md_line_FS(dev, x, y, th) YPSpur_md_line(dev, CS_FS, x, y, th)
#define Spur_md_line_BS(dev, x, y, th) YPSpur_md_line(dev, CS_BS, x, y, th)
#define Spur_md_line_BL(dev, x, y, th) YPSpur_md_line(dev, CS_BL, x, y, th)

#define Spur_md_stop_line_GL(dev, x, y, th) YPSpur_md_stop_line(dev, CS_GL, x, y, th)
#define Spur_md_stop_line_LC(dev, x, y, th) YPSpur_md_stop_line(dev, CS_LC, x, y, th)
#define Spur_md_stop_line_FS(dev, x, y, th) YPSpur_md_stop_line(dev, CS_FS, x, y, th)
#define Spur_md_stop_line_BS(dev, x, y, th) YPSpur_md_stop_line(dev, CS_BS, x, y, th)
#define Spur_md_stop_line_BL(dev, x, y, th) YPSpur_md_stop_line(dev, CS_BL, x, y, th)

#define Spur_md_circle_GL(dev, x, y, d) YPSpur_md_circle(dev, CS_GL, x, y, d)
#define Spur_md_circle_LC(dev, x, y, d) YPSpur_md_circle(dev, CS_LC, x, y, d)
#define Spur_md_circle_FS(dev, x, y, d) YPSpur_md_circle(dev, CS_FS, x, y, d)
#define Spur_md_circle_BS(dev, x, y, d) YPSpur_md_circle(dev, CS_BS, x, y, d)
#define Spur_md_circle_BL(dev, x, y, d) YPSpur_md_circle(dev, CS_BL, x, y, d)

#define Spur_md_spin_GL(dev, th) YPSpur_md_spin(dev, CS_GL, th)
#define Spur_md_spin_LC(dev, th) YPSpur_md_spin(dev, CS_LC, th)
#define Spur_md_spin_FS(dev, th) YPSpur_md_spin(dev, CS_FS, th)
#define Spur_md_spin_BS(dev, th) YPSpur_md_spin(dev, CS_BS, th)
#define Spur_md_spin_BL(dev, th) YPSpur_md_spin(dev, CS_BL, th)

#define Spur_md_orient_GL(dev, th) YPSpur_md_orient(dev, CS_GL, th)
#define Spur_md_orient_LC(dev, th) YPSpur_md_orient(dev, CS_LC, th)
#define Spur_md_orient_FS(dev, th) YPSpur_md_orient(dev, CS_FS, th)
#define Spur_md_orient_BS(dev, th) YPSpur_md_orient(dev, CS_BS, th)
#define Spur_md_orient_BL(dev, th) YPSpur_md_orient(dev, CS_BL, th)

#define Spur_md_set_pos_GL(dev, x, y, th) YPSpur_md_set_pos(dev, CS_GL, x, y, th)
#define Spur_md_set_pos_LC(dev, x, y, th) YPSpur_md_set_pos(dev, CS_LC, x, y, th)
#define Spur_md_set_pos_BL(dev, x, y, th) YPSpur_md_set_pos(dev, CS_BL, x, y, th)

#define Spur_md_set_vel(dev, v) YPSpur_md_set_vel(dev, v)
#define Spur_md_set_angvel(dev, w) YPSpur_md_set_angvel(dev, w)
#define Spur_md_set_accel(dev, v) YPSpur_md_set_accel(dev, v)
#define Spur_md_set_angaccel(dev, w) YPSpur_md_set_angaccel(dev, w)

#define Spur_md_adjust_pos_GL(dev, x, y, th) YPSpur_md_adjust_pos(dev, CS_GL, x, y, th)
#define Spur_md_adjust_pos_LC(dev, x, y, th) YPSpur_md_adjust_pos(dev, CS_LC, x, y, th)
#define Spur_md_adjust_pos_FS(dev, x, y, th) YPSpur_md_adjust_pos(dev, CS_FS, x, y, th)
#define Spur_md_adjust_pos_BS(dev, x, y, th) YPSpur_md_adjust_pos(dev, CS_BS, x, y, th)
#define Spur_md_adjust_pos_BL(dev, x, y, th) YPSpur_md_adjust_pos(dev, CS_BL, x, y, th)

#define Spur_md_get_pos_GL(dev, x, y, th) YPSpur_md_get_pos(dev, CS_GL, x, y, th)
#define Spur_md_get_pos_LC(dev, x, y, th) YPSpur_md_get_pos(dev, CS_LC, x, y, th)
#define Spur_md_get_pos_BS(dev, x, y, th) YPSpur_md_get_pos(dev, CS_BS, x, y, th)
#define Spur_md_get_pos_BL(dev, x, y, th) YPSpur_md_get_pos(dev, CS_BL, x, y, th)

#define Spur_md_get_vel(dev, v, w) YPSpur_md_get_vel(dev, v, w)
#define Spur_md_get_force(dev, trans, angular) YPSpur_md_get_force(dev, trans, angular)

#define Spur_md_near_pos_GL(dev, x, y, r) YPSpur_md_near_pos(dev, CS_GL, x, y, r)
#define Spur_md_near_pos_LC(dev, x, y, r) YPSpur_md_near_pos(dev, CS_LC, x, y, r)
#define Spur_md_near_pos_BS(dev, x, y, r) YPSpur_md_near_pos(dev, CS_BS, x, y, r)
#define Spur_md_near_pos_BL(dev, x, y, r) YPSpur_md_near_pos(dev, CS_BL, x, y, r)

#define Spur_md_near_ang_GL(dev, th, d) YPSpur_md_near_ang(dev, CS_GL, th, d)
#define Spur_md_near_ang_LC(dev, th, d) YPSpur_md_near_ang(dev, CS_LC, th, d)
#define Spur_md_near_ang_BS(dev, th, d) YPSpur_md_near_ang(dev, CS_BS, th, d)
#define Spur_md_near_ang_BL(dev, th, d) YPSpur_md_near_ang(dev, CS_BL, th, d)

#define Spur_md_over_line_GL(dev, x, y, th) YPSpur_md_over_line(dev, CS_GL, x, y, th)
#define Spur_md_over_line_LC(dev, x, y, th) YPSpur_md_over_line(dev, CS_LC, x, y, th)
#define Spur_md_over_line_BS(dev, x, y, th) YPSpur_md_over_line(dev, CS_BS, x, y, th)
#define Spur_md_over_line_BL(dev, x, y, th) YPSpur_md_over_line(dev, CS_BL, x, y, th)

#define Spur_md_vel(dev, v, w) YPSpur_md_vel(dev, v, w)

#define Spur_md_tilt_GL(dev, d, t) YPSpur_md_tilt(dev, CS_GL, d, t)
#define Spur_md_tilt_LC(dev, d, t) YPSpur_md_tilt(dev, CS_LC, d, t)
#define Spur_md_tilt_FS(dev, d, t) YPSpur_md_tilt(dev, CS_FS, d, t)
#define Spur_md_tilt_BS(dev, d, t) YPSpur_md_tilt(dev, CS_BS, d, t)
#define Spur_md_tilt_BL(dev, d, t) YPSpur_md_tilt(dev, CS_BL, d, t)

typedef struct
{
  struct ipcmd_t dev;
  int pid;
  int connection_error;
} YPSpur;

/** init */
int YPSpur_md_init(YPSpur *dev);
int YPSpur_md_initex(YPSpur *dev, int msq_key);
int YPSpur_md_init_socket(YPSpur *dev, char *ip, int port);

/** freeze */
int YPSpur_md_isfreeze(YPSpur *dev);
int YPSpur_md_freeze(YPSpur *dev);
int YPSpur_md_unfreeze(YPSpur *dev);

/** motor servo */
int YPSpur_md_stop(YPSpur *dev);
int YPSpur_md_free(YPSpur *dev);
int YP_md_openfree(YPSpur *dev);

/** Spur_md */
int YPSpur_md_line(YPSpur *dev, int cs, double x, double y, double theta);
int YPSpur_md_stop_line(YPSpur *dev, int cs, double x, double y, double theta);
int YPSpur_md_circle(YPSpur *dev, int cs, double x, double y, double r);
int YPSpur_md_spin(YPSpur *dev, int cs, double theta);
int YPSpur_md_orient(YPSpur *dev, int cs, double theta);

/** setter */
int YPSpur_md_set_pos(YPSpur *dev, int cs, double x, double y, double theta);
int YPSpur_md_adjust_pos(YPSpur *dev, int cs, double x, double y, double theta);
int YPSpur_md_set_vel(YPSpur *dev, double v);
int YPSpur_md_set_angvel(YPSpur *dev, double w);
int YPSpur_md_set_accel(YPSpur *dev, double v);
int YPSpur_md_set_angaccel(YPSpur *dev, double w);

/** getter */
double YPSpur_md_get_pos(YPSpur *dev, int cs, double *x, double *y, double *theta);
double YPSpur_md_get_vel(YPSpur *dev, double *v, double *w);
double YPSpur_md_get_force(YPSpur *dev, double *trans, double *angular);

/** check position */
int YPSpur_md_near_pos(YPSpur *dev, int cs, double x, double y, double r);
int YPSpur_md_near_ang(YPSpur *dev, int cs, double th, double d);
int YPSpur_md_over_line(YPSpur *dev, int cs, double x, double y, double theta);

/** direct */
int YPSpur_md_vel(YPSpur *dev, double v, double w);

/** tilt */
int YPSpur_md_tilt(YPSpur *dev, int cs, double dir, double tilt);

/* 裏コマンド集 */
int YP_md_get_error_state(YPSpur *dev);
void YP_md_request_device_dump(YPSpur *dev, int id, int block);

int YP_md_set_parameter(YPSpur *dev, int param_id, double value);
int YP_md_set_parameter_array(YPSpur *dev, int param_id, double *value);
int YP_md_get_parameter(YPSpur *dev, int param_id, double *value);
int YP_md_get_parameter_array(YPSpur *dev, int param_id, double *value);
int YP_md_set_control_state(YPSpur *dev, int control_id, int state);

int YP_md_get_ad_value(YPSpur *dev, int num);
int YP_md_set_io_dir(YPSpur *dev, unsigned char dir);
int YP_md_set_io_data(YPSpur *dev, unsigned char data);
double YP_md_get_device_error_state(YPSpur *dev, int id, int *err);
int YP_md_wheel_vel(YPSpur *dev, double r, double l);
int YP_md_wheel_torque(YPSpur *dev, double r, double l);
double YP_md_get_wheel_vel(YPSpur *dev, double *wr, double *wl);
double YP_md_get_wheel_ang(YPSpur *dev, double *theta_r, double *theta_l);
double YP_md_get_wheel_torque(YPSpur *dev, double *torque_r, double *torque_l);
int YP_md_set_wheel_vel(YPSpur *dev, double r, double l);
int YP_md_set_wheel_accel(YPSpur *dev, double r, double l);
int YP_md_wheel_ang(YPSpur *dev, double r, double l);
double YP_md_get_vref(YPSpur *dev, double *vref, double *wref);
double YP_md_get_wheel_vref(YPSpur *dev, double *wrref, double *wlref);

int YP_md_joint_torque(YPSpur *spur, int id, double t);
int YP_md_joint_vel(YPSpur *spur, int id, double v);
int YP_md_joint_ang(YPSpur *spur, int id, double a);
int YP_md_joint_ang_vel(YPSpur *spur, int id, double a, double v);
int YP_md_set_joint_accel(YPSpur *spur, int id, double a);
int YP_md_set_joint_vel(YPSpur *spur, int id, double v);
double YP_md_get_joint_vel(YPSpur *spur, int id, double *v);
double YP_md_get_joint_vref(YPSpur *spur, int id, double *v);
double YP_md_get_joint_ang(YPSpur *spur, int id, double *a);
double YP_md_get_joint_torque(YPSpur *spur, int id, double *t);

#define YPSPUR_JOINT_SUPPORT 1
#define YPSPUR_JOINT_ANG_VEL_SUPPORT 1
#define YPSPUR_GET_DEVICE_ERROR_STATE_SUPPORT 1

#ifdef __cplusplus
}
#endif  // __cplusplus
#endif  // YPSPUR_MD_H

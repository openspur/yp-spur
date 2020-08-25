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

#ifndef COMMAND_H
#define COMMAND_H

#include <ipcommunication.h>
#include <ypparam.h>
#include <ypspur.h>

typedef enum
{
  RUN_OPENFREE = -1,
  RUN_FREE = 0,
  RUN_STOP,
  RUN_LINEFOLLOW,
  RUN_STOP_LINE,
  RUN_CIRCLEFOLLOW,
  RUN_SPIN,
  RUN_ORIENT,
  RUN_VEL,
  RUN_WHEEL_VEL,
  RUN_WHEEL_TORQUE,
  RUN_WHEEL_ANGLE
} SpurRunMode;

typedef enum
{
  MOTOR_CONTROL_OPENFREE,
  MOTOR_CONTROL_FREE,
  MOTOR_CONTROL_VEHICLE,
  MOTOR_CONTROL_VEL,
  MOTOR_CONTROL_ANGLE,
  MOTOR_CONTROL_ANGLE_VEL
} MotorControlMode;

typedef struct _spur_user_params *SpurUserParamsPtr;
typedef struct _spur_user_params
{
  double dv;
  double dw;
  double v;
  double w;
  double vref;
  double wref;
  double vref_smooth;
  double wref_smooth;
  double wvelref[YP_PARAM_MAX_MOTOR_NUM];
  double x;
  double y;
  double torque[YP_PARAM_MAX_MOTOR_NUM];
  double torque_prev[YP_PARAM_MAX_MOTOR_NUM];
  double grav_torque[YP_PARAM_MAX_MOTOR_NUM];
  double wheel_accel[YP_PARAM_MAX_MOTOR_NUM];
  double wheel_vel[YP_PARAM_MAX_MOTOR_NUM];
  double wheel_vel_smooth[YP_PARAM_MAX_MOTOR_NUM];
  double wheel_angle[YP_PARAM_MAX_MOTOR_NUM];
  double wheel_vel_end[YP_PARAM_MAX_MOTOR_NUM];
  MotorControlMode wheel_mode[YP_PARAM_MAX_MOTOR_NUM];
  MotorControlMode wheel_mode_prev[YP_PARAM_MAX_MOTOR_NUM];
  double theta;
  double radius;
  double tilt;
  double dir;
  double control_dt;
  int freeze;
  int before_freeze;
  SpurRunMode run_mode;
  int run_mode_cnt;
  SpurRunMode before_run_mode;
  pthread_mutex_t mutex;
} SpurUserParams;

void init_spur_command(void);
void command(void);
void message_return(struct ipcmd_t *ipcmd, long retpid, YPSpur_msg *res_msg);

void init_command_thread(pthread_t *thread);
SpurUserParamsPtr get_spur_user_param_ptr();

/* command_run.c */
void line_com(int cs, double *data, SpurUserParamsPtr spur);
void stop_line_com(int cs, double *data, SpurUserParamsPtr spur);
void circle_com(int cs, double *data, SpurUserParamsPtr spur);
void spin_com(int cs, double *data, SpurUserParamsPtr spur);
void orient_com(int cs, double *data, SpurUserParamsPtr spur);
void stop_com(double *data, SpurUserParamsPtr spur);
void free_com(double *data, SpurUserParamsPtr spur);
void openfree_com(double *data, SpurUserParamsPtr spur);
void vel_com(double *data, SpurUserParamsPtr spur);
void wheel_vel_com(double *data, SpurUserParamsPtr spur);
void wheel_angle_com(double *data, SpurUserParamsPtr spur);

/* command_set.c */
void set_pos_com(int cs, double *data, SpurUserParamsPtr spur);
void set_GL_on_GL_com(double *data, SpurUserParamsPtr spur);
void set_adjust_com(int cs, double *data, SpurUserParamsPtr spur);
void set_vel_com(double *data, SpurUserParamsPtr spur);
void set_ang_accel_com(double *data, SpurUserParamsPtr spur);
void set_accel_com(double *data, SpurUserParamsPtr spur);
void set_ang_vel_com(double *data, SpurUserParamsPtr spur);
void set_tilt_com(int cs, double *data, SpurUserParamsPtr spur);
void set_torque_com(double *data, SpurUserParamsPtr spur);
void set_wheel_vel_com(double *data, SpurUserParamsPtr spur);
void set_wheel_accel_com(double *data, SpurUserParamsPtr spur);

/* command_get.c */
void get_pos_com(int cs, double *data, double *resdata, SpurUserParamsPtr spur);
void get_vel_com(int cs, double *data, double *resdata, SpurUserParamsPtr spur);
void get_vref_com(int cs, double *data, double *resdata, SpurUserParamsPtr spur);
void get_wheel_vref_com(int cs, double *data, double *resdata, SpurUserParamsPtr spur);
int near_pos_com(int cs, double *data, double *resdata, SpurUserParamsPtr spur);
int near_ang_com(int cs, double *data, double *resdata, SpurUserParamsPtr spur);
int over_line_com(int cs, double *data, double *resdata, SpurUserParamsPtr spur);
void get_wheel_vel_com(double *data, double *resdata, SpurUserParamsPtr spur);
void get_wheel_ang_com(double *data, double *resdata, SpurUserParamsPtr spur);
void get_force_com(int cs, double *data, double *resdata, SpurUserParamsPtr spur);
void get_wheel_torque_com(int cs, double *data, double *resdata, SpurUserParamsPtr spur);

/* command_param.c */
void param_set_com(int cs, double *data, SpurUserParamsPtr spur);
int param_get_com(int cs, double *resdata, SpurUserParamsPtr spur);
void param_state_com(int cs, double *data, SpurUserParamsPtr spur);

/* command_ad.c */
void get_ad_com(double *data, double *resdata);
void set_io_dir_com(double *data, double *resdata);
void set_io_data_com(double *data, double *resdata);

void joint_torque_com(int id, double *data, SpurUserParamsPtr spur);
void joint_vel_com(int id, double *data, SpurUserParamsPtr spur);
void joint_ang_com(int id, double *data, SpurUserParamsPtr spur);
void joint_ang_vel_com(int id, double *data, SpurUserParamsPtr spur);
void set_joint_accel_com(int id, double *data, SpurUserParamsPtr spur);
void set_joint_vel_com(int id, double *data, SpurUserParamsPtr spur);
void get_joint_vel_com(int id, double *data, SpurUserParamsPtr spur);
void get_joint_vref_com(int id, double *data, SpurUserParamsPtr spur);
void get_joint_ang_com(int id, double *data, SpurUserParamsPtr spur);
void get_joint_torque_com(int id, double *data, SpurUserParamsPtr spur);

void get_error_state_com(double *data, double *resdata);
void dump_device_com(double *data, double *resdata);

#endif  // COMMAND_H

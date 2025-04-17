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

#ifndef ODOMETRY_H
#define ODOMETRY_H

#include <shvel-param.h>

#include <cartesian2d.h>
#include <ypparam.h>
#include <ypspur.h>

typedef struct _odometry* OdometryPtr;
typedef struct _odometry
{
  double x;
  double y;
  double theta;
  double v;
  double w;
  double time;
  double wvel[YP_PARAM_MAX_MOTOR_NUM];
  double wang[YP_PARAM_MAX_MOTOR_NUM];
  short enc[YP_PARAM_MAX_MOTOR_NUM];
  int enc_init[YP_PARAM_MAX_MOTOR_NUM];
  double wtorque[YP_PARAM_MAX_MOTOR_NUM];
  double torque_trans;
  double torque_angular;
  int ping_response[YP_PARAM_MAX_MOTOR_NUM + 1];
  int packet_lost;
  int packet_lost_last;
} Odometry;

typedef struct _error_state* ErrorStatePtr;
typedef struct _error_state
{
  YPSpur_shvel_error_state state[YP_PARAM_MAX_MOTOR_NUM];
  double time[YP_PARAM_MAX_MOTOR_NUM];
} ErrorState;

double time_estimate(int readnum);
void cstrans_odometry(YPSpur_cs cs, OdometryPtr dst_odm);
void cstrans_xy(YPSpur_cs src, YPSpur_cs dest, double* x, double* y, double* theta);
void odometry(OdometryPtr xp, short* cnt, short* pwm, double dt, double time);
void process_int(
    OdometryPtr xp, ErrorStatePtr err, int param_id, int id, int value, double receive_time);
void odm_logging(OdometryPtr, double, double);
int odm_read(OdometryPtr odm, double* v, double* w);
void cs_odometry(YPSpur_cs cs, OdometryPtr dst_odm);
void init_odometry(void);
void init_coordinate_systems(void);
int odometry_receive_loop(void);
OdometryPtr get_odometry_ptr();
ErrorStatePtr get_error_state_ptr();
CSptr get_cs_pointer(YPSpur_cs cs);
void set_cs(YPSpur_cs cs, double x, double y, double theta);

#endif  // ODOMETRY_H

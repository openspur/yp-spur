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

#ifndef YPSPUR_ODOMETRY_TYPE_H
#define YPSPUR_ODOMETRY_TYPE_H

#include <ypspur/error_type.h>
#include <ypspur/ypparam.h>

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

typedef struct _odometry* OdometryPtr;
typedef struct _odometry
{
  // Updated by isochronous communication
  double time;
  double x;
  double y;
  double theta;
  double v;
  double w;
  double wang[YP_PARAM_MAX_MOTOR_NUM];
  double wvel[YP_PARAM_MAX_MOTOR_NUM];
  double wtorque[YP_PARAM_MAX_MOTOR_NUM];
  short enc[YP_PARAM_MAX_MOTOR_NUM];
  int enc_init[YP_PARAM_MAX_MOTOR_NUM];
  double torque_trans;
  double torque_angular;
  int packet_lost;
  int packet_lost_last;

  // Updated by interrupt communication
  double wang_time[YP_PARAM_MAX_MOTOR_NUM];
  int ping_response[YP_PARAM_MAX_MOTOR_NUM + 1];
  double ping_response_time[YP_PARAM_MAX_MOTOR_NUM + 1];
} Odometry;

typedef struct _error_state* ErrorStatePtr;
typedef struct _error_state
{
  YPSpur_shvel_error_state state[YP_PARAM_MAX_MOTOR_NUM];
  double time[YP_PARAM_MAX_MOTOR_NUM];
} ErrorState;

typedef void (*OdometryHook)(const OdometryPtr, const ErrorStatePtr);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // YPSPUR_ODOMETRY_TYPE_H

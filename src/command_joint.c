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

#include <math.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif  // HAVE_CONFIG_H

/* ボディパラメータ */
#include <shvel-param.h>

/* yp-spur用 */
#include <param.h>
#include <control.h>
#include <command.h>
#include <yprintf.h>

/* ライブラリ用 */
#include <ypspur.h>

void joint_torque_com(int id, double *data, SpurUserParamsPtr spur)
{
  if (id > YP_PARAM_MAX_MOTOR_NUM)
  {
    yprintf(OUTPUT_LV_ERROR, "Motor id out of range (%d)\n", id);
    return;
  }
  spur->torque[id] = data[0];
  spur->wheel_mode[id] = MOTOR_CONTROL_FREE;
}

void joint_vel_com(int id, double *data, SpurUserParamsPtr spur)
{
  if (id > YP_PARAM_MAX_MOTOR_NUM)
  {
    yprintf(OUTPUT_LV_ERROR, "Motor id out of range (%d)\n", id);
    return;
  }
  spur->wvelref[id] = data[0];
  spur->wheel_mode[id] = MOTOR_CONTROL_VEL;
}

void joint_ang_com(int id, double *data, SpurUserParamsPtr spur)
{
  if (id > YP_PARAM_MAX_MOTOR_NUM)
  {
    yprintf(OUTPUT_LV_ERROR, "Motor id out of range (%d)\n", id);
    return;
  }
  spur->wheel_angle[id] = data[0];
  spur->wheel_mode[id] = MOTOR_CONTROL_ANGLE;
}

void joint_ang_vel_com(int id, double *data, SpurUserParamsPtr spur)
{
  if (id > YP_PARAM_MAX_MOTOR_NUM)
  {
    yprintf(OUTPUT_LV_ERROR, "Motor id out of range (%d)\n", id);
    return;
  }
  spur->wheel_angle[id] = data[0];
  spur->wheel_vel_end[id] = data[1];
  spur->wheel_mode[id] = MOTOR_CONTROL_ANGLE_VEL;
}

void set_joint_accel_com(int id, double *data, SpurUserParamsPtr spur)
{
  if (id > YP_PARAM_MAX_MOTOR_NUM)
  {
    yprintf(OUTPUT_LV_ERROR, "Motor id out of range (%d)\n", id);
    return;
  }
  spur->wheel_accel[id] = data[0];
}

void set_joint_vel_com(int id, double *data, SpurUserParamsPtr spur)
{
  if (id > YP_PARAM_MAX_MOTOR_NUM)
  {
    yprintf(OUTPUT_LV_ERROR, "Motor id out of range (%d)\n", id);
    return;
  }
  spur->wheel_vel[id] = data[0];
}

void get_joint_vel_com(int id, double *data, SpurUserParamsPtr spur)
{
  OdometryPtr odometry;

  if (id > YP_PARAM_MAX_MOTOR_NUM)
  {
    yprintf(OUTPUT_LV_ERROR, "Motor id out of range (%d)\n", id);
    return;
  }
  odometry = get_odometry_ptr();

  data[0] = odometry->wvel[id];
  data[1] = odometry->time;
}

void get_joint_vref_com(int id, double *data, SpurUserParamsPtr spur)
{
  OdometryPtr odometry;

  if (id > YP_PARAM_MAX_MOTOR_NUM)
  {
    yprintf(OUTPUT_LV_ERROR, "Motor id out of range (%d)\n", id);
    return;
  }
  odometry = get_odometry_ptr();

  data[0] = spur->wvelref[id];
  data[1] = odometry->time;
}

void get_joint_ang_com(int id, double *data, SpurUserParamsPtr spur)
{
  OdometryPtr odometry;

  if (id > YP_PARAM_MAX_MOTOR_NUM)
  {
    yprintf(OUTPUT_LV_ERROR, "Motor id out of range (%d)\n", id);
    return;
  }
  odometry = get_odometry_ptr();

  data[0] = odometry->wang[id];
  data[1] = odometry->time;
}

void get_joint_torque_com(int id, double *data, SpurUserParamsPtr spur)
{
  OdometryPtr odometry;

  if (id > YP_PARAM_MAX_MOTOR_NUM)
  {
    yprintf(OUTPUT_LV_ERROR, "Motor id out of range (%d)\n", id);
    return;
  }
  odometry = get_odometry_ptr();

  data[0] = odometry->wtorque[id];
  data[1] = odometry->time;
}

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
#include <serial.h>
#include <param.h>
#include <command.h>
#include <odometry.h>
#include <control.h>
#include <utility.h>
#include <yprintf.h>

/* ライブラリ用 */
#include <ypspur.h>
#include <cartesian2d.h>

void param_set_com(int cs, double *data, SpurUserParamsPtr spur)
{
  if (cs >= 0 && cs < YP_PARAM_NUM)
  {
    *pp(cs, 0) = data[0];
    *pp(cs, 1) = data[1];
  }
}

int param_get_com(int cs, double *resdata, SpurUserParamsPtr spur)
{
  if (cs >= 0 && cs < YP_PARAM_NUM)
  {
    resdata[0] = *pp(cs, 0);
    resdata[1] = *pp(cs, 1);
    return cs;
  }
  else
  {
    return 0;
  }
}

void param_set_motor_com(int cs, double *data, SpurUserParamsPtr spur)
{
  if (cs >= 0 && cs < YP_PARAM_NUM)
    *pp(cs, data[0]) = data[1];
}

int param_get_motor_com(int cs, double *resdata, SpurUserParamsPtr spur)
{
  if (cs >= 0 && cs < YP_PARAM_NUM)
  {
    resdata[0] = *pp(cs, resdata[0]);
    return cs;
  }
  else
  {
    return 0;
  }
}

void param_state_com(int cs, double *data, SpurUserParamsPtr spur)
{
  OdometryPtr odometry;

  odometry = get_odometry_ptr();

  if (cs >= 0 && cs < YP_STATE_NUM)
  {
    if (data[0])
    {
      enable_state(cs);
    }
    else
    {
      disable_state(cs);
    }
  }

  if (cs == YP_STATE_MOTOR && data[0] == ENABLE)
  {
    set_param_motor();
    yprintf(OUTPUT_LV_DEBUG, "COMMAND: Motor Control: enabled\n");
  }
  if (cs == YP_STATE_VELOCITY && data[0] == ENABLE)
  {
    set_param_velocity();
    yprintf(OUTPUT_LV_DEBUG, "COMMAND: Velocity Control: enabled\n");
  }
  if (cs == YP_STATE_BODY && data[0] == ENABLE)
  {
    spur->vref_smooth = 0;
    spur->wref_smooth = 0;
    robot_speed(spur);
    odometry->x = 0;
    odometry->y = 0;
    odometry->theta = 0;
    yprintf(OUTPUT_LV_DEBUG, "COMMAND: Body Control: enabled\n");
  }
  if (cs == YP_STATE_TRACKING && data[0] == ENABLE)
  {
    yprintf(OUTPUT_LV_DEBUG, "COMMAND: Trajectory Control: enabled\n");
  }
  if (cs == YP_STATE_GRAVITY && data[0] == ENABLE)
  {
    yprintf(OUTPUT_LV_DEBUG, "COMMAND: Gravity Compensation: enabled\n");
  }
}

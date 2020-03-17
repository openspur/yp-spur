// Copyright (c) 2020 The YP-Spur Authors, except where otherwise indicated.
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

#include <param.h>
#include <utility.h>
#include <odometry.h>
#include <shvel-param.h>
#include <yprintf.h>

int ping()
{
  const int data = 0x123456;
  yprintf(OUTPUT_LV_INFO, "Ping request: 0x%08x\n", data);

  OdometryPtr odom = get_odometry_ptr();
  int i;
  for (i = 0; i < YP_PARAM_MAX_MOTOR_NUM + 1; i++)
    odom->ping_response[i] = 0;

  const char target = MOTOR_ID_BROADCAST;
  parameter_set(PARAM_servo, target, 0);
  parameter_set(PARAM_ping, target, data);
  odometry_receive_loop();

  int ret = 1;
  for (i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i++)
  {
    if (odom->ping_response[i] == data)
    {
      ret = 0;
      yprintf(OUTPUT_LV_INFO, "Ping response from ID: %d\n", i);
    }
  }
  if (odom->ping_response[i] == data)
  {
    ret = 0;
    yprintf(OUTPUT_LV_INFO, "Ping response from device without ID\n");
  }
  if (ret)
  {
    yprintf(OUTPUT_LV_ERROR, "No ping response!\n");
  }

  return 1;
}

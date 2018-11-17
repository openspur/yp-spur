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

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <ypspur.h>

int main(int argc, char *argv[])
{
  double refvel;
  double torque, force;
  double torque_sum;
  double torque1;
  double alpha;
  int torque_num;

  /* 初期化 */
  if (Spur_init() < 0)
  {
    fprintf(stderr, "ERROR : cannot open spur.\n");
    return -1;
  }

  if (argc != 3)
  {
    fprintf(stderr, "USAGE: %s maximum_velocity second\n", argv[0]);
    return -1;
  }

  refvel = atoi(argv[1]);
  alpha = refvel / atoi(argv[2]);

  usleep(1000000);
  Spur_set_vel(0);
  Spur_set_accel(0);
  Spur_set_angvel(refvel);
  Spur_set_angaccel(alpha);

  Spur_vel(0, M_PI);
  torque_sum = 0;
  torque_num = 0;
  torque1 = 0;

  Spur_set_pos_GL(0, 0, 0);

  printf("# torque force torque_r, torque_l vel w\n");
  while (1)
  {
    double vel, w;

    Spur_get_vel(&vel, &w);
    if (w > refvel * 0.9)
      break;
    if (w > refvel * 0.4)
    {
      double tr, tl;
      Spur_get_force(&force, &torque);
      if (torque1 == 0)
        torque1 = torque;
      YP_get_wheel_torque(&tr, &tl);
      torque_sum += torque;
      torque_num++;
      printf("%f %f  %f %f  %f %f\n", torque, force, tr, tl, vel, w);
    }
    usleep(5000);
  }
  Spur_set_angaccel(2);
  Spur_spin_GL(0);

  fprintf(stderr, "INFO: Every robot parameter MUST BE TUNED FINELY!\n");
  fprintf(stderr, "INFO: This estimation is susceptible to ERROR!\n");

  if (fabs((torque1 - torque) / torque1) > 0.5)
  {
    if (torque1 > torque)
    {
      fprintf(stderr, "WARNING: TORQUE_VISCOS or 1/MOTOR_VC might be too large\n");
    }
    else
    {
      fprintf(stderr, "WARNING: TORQUE_VISCOS or 1/MOTOR_VC might be too small\n");
    }
  }

  {
    double I, N;

    if (alpha < 0.4)
    {
      fprintf(stderr, "WARNING: Too small Angular acceleration\n");
    }

    N = (torque_sum / torque_num);  //[Nm]

    // I alpha = N
    I = N / alpha;

    fprintf(stderr, "%f[Nm], %f[rad/s^2] %f[Nm^2]\n", N, alpha, I);
  }

  return 0;
}

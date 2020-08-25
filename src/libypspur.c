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

/* 
 * YP-Spurとの通信ライブラリ Communication Library for YP-Spur
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif  // HAVE_CONFIG_H

#define YP_SPUR_RUNTIME

#include <ipcommunication.h>
#include <ypspur.h>
#include <ypspur-md.h>

static YPSpur spur;

/* エラー確認 */
int YP_get_error_state()
{
  return YP_md_get_error_state(&spur);
}

double YP_get_device_error_state(int id, int *err)
{
  return YP_md_get_device_error_state(&spur, id, err);
}

/* coordinatorとのメッセージ通信を開始する */
int YPSpur_initex(int msq_key)
{
  return YPSpur_md_initex(&spur, msq_key);
}

int YPSpur_init_socket(char *ip, int port)
{
  return YPSpur_md_init_socket(&spur, ip, port);
}

/* coordinatorとのメッセージ通信を開始する */
int YPSpur_init(void)
{
  return YPSpur_md_init(&spur);
}

/* 直線追従 */
int YPSpur_line(int cs, double x, double y, double theta)
{
  return YPSpur_md_line(&spur, cs, x, y, theta);
}

/* 直線追従 */
int YPSpur_stop_line(int cs, double x, double y, double theta)
{
  return YPSpur_md_stop_line(&spur, cs, x, y, theta);
}

/* 円弧追従 */
int YPSpur_circle(int cs, double x, double y, double r)
{
  return YPSpur_md_circle(&spur, cs, x, y, r);
}

/* 旋回 */
int YPSpur_spin(int cs, double theta)
{
  return YPSpur_md_spin(&spur, cs, theta);
}

/* 方位 */
int YPSpur_orient(int cs, double theta)
{
  return YPSpur_md_orient(&spur, cs, theta);
}

/* 急ブレーキ */
int YPSpur_stop(void)
{
  return YPSpur_md_stop(&spur);
}

/* 緊急停止 */
int YPSpur_freeze(void)
{
  return YPSpur_md_freeze(&spur);
}

/* 緊急停止解除 */
int YPSpur_unfreeze(void)
{
  return YPSpur_md_unfreeze(&spur);
}

/* ソフトウェア補助フリーモード */
int YPSpur_free(void)
{
  return YPSpur_md_free(&spur);
}

/* 制御なしフリーモード */
int YP_openfree(void)
{
  return YP_md_openfree(&spur);
}

/* 位置指定 */
int YPSpur_set_pos(int cs, double x, double y, double theta)
{
  return YPSpur_md_set_pos(&spur, cs, x, y, theta);
}

/* 位置指定 */
int YPSpur_adjust_pos(int cs, double x, double y, double theta)
{
  return YPSpur_md_adjust_pos(&spur, cs, x, y, theta);
}

/* 速度指定 */
int YPSpur_set_vel(double v)
{
  return YPSpur_md_set_vel(&spur, v);
}

/* 角速度指定 */
int YPSpur_set_angvel(double w)
{
  return YPSpur_md_set_angvel(&spur, w);
}

/* 速度指定 */
int YPSpur_set_accel(double dv)
{
  return YPSpur_md_set_accel(&spur, dv);
}

/* 角速度指定 */
int YPSpur_set_angaccel(double dw)
{
  return YPSpur_md_set_angaccel(&spur, dw);
}

/* 位置取得 */
double YPSpur_get_pos(int cs, double *x, double *y, double *theta)
{
  return YPSpur_md_get_pos(&spur, cs, x, y, theta);
}

/* 速度取得 */
double YPSpur_get_vel(double *v, double *w)
{
  return YPSpur_md_get_vel(&spur, v, w);
}

/* 速度取得 */
double YP_get_vref(double *v, double *w)
{
  return YP_md_get_vref(&spur, v, w);
}

/* 速度取得 */
double YP_get_wheel_vref(double *wr, double *wl)
{
  return YP_md_get_wheel_vref(&spur, wr, wl);
}

/* 速度取得 */
double YP_get_wheel_vel(double *wr, double *wl)
{
  return YP_md_get_wheel_vel(&spur, wr, wl);
}

/* 角度取得 */
double YP_get_wheel_ang(double *theta_r, double *theta_l)
{
  return YP_md_get_wheel_ang(&spur, theta_r, theta_l);
}

/* トルク取得 */
double YP_get_wheel_torque(double *torque_r, double *torque_l)
{
  return YP_md_get_wheel_torque(&spur, torque_r, torque_l);
}

/* 力取得 */
double YPSpur_get_force(double *trans, double *angular)
{
  return YPSpur_md_get_force(&spur, trans, angular);
}

/* 緊急停止状態取得 */
int YPSpur_isfreeze(void)
{
  return YPSpur_md_isfreeze(&spur);
}

/* 直接速度入力 */
int YPSpur_vel(double v, double w)
{
  return YPSpur_md_vel(&spur, v, w);
}

/* 内部パラメータの変更 */
int YP_set_parameter(int param_id, double value)
{
  return YP_md_set_parameter(&spur, param_id, value);
}

/* 内部パラメータの変更 */
int YP_set_parameter_array(int param_id, double *value)
{
  return YP_md_set_parameter_array(&spur, param_id, value);
}

/* 内部パラメータの取得 */
int YP_get_parameter(int param_id, double *value)
{
  return YP_md_get_parameter(&spur, param_id, value);
}

/* 内部パラメータの取得 */
int YP_get_parameter_array(int param_id, double *value)
{
  return YP_md_get_parameter_array(&spur, param_id, value);
}

/* 内部状態の変更 */
int YP_set_control_state(int control_id, int state)
{
  return YP_md_set_control_state(&spur, control_id, state);
}

/* 重力補償用地面の傾き指定 */
int YPSpur_tilt(int cs, double dir, double tilt)
{
  return YPSpur_md_tilt(&spur, cs, dir, tilt);
}

/* 位置判定 */
int YPSpur_near_pos(int cs, double x, double y, double r)
{
  return YPSpur_md_near_pos(&spur, cs, x, y, r);
}

/* 角度判定 */
int YPSpur_near_ang(int cs, double th, double d)
{
  return YPSpur_md_near_ang(&spur, cs, th, d);
}

/* 領域判定 */
int YPSpur_over_line(int cs, double x, double y, double theta)
{
  return YPSpur_md_over_line(&spur, cs, x, y, theta);
}

/* アナログ値取得 */
int YP_get_ad_value(int num)
{
  return YP_md_get_ad_value(&spur, num);
}

int YP_set_io_dir(unsigned char dir)
{
  return YP_md_set_io_dir(&spur, dir);
}

int YP_set_io_data(unsigned char data)
{
  return YP_md_set_io_data(&spur, data);
}

/* 直接タイヤ回転速度入力 */
int YP_wheel_vel(double r, double l)
{
  return YP_md_wheel_vel(&spur, r, l);
}

int YP_wheel_torque(double r, double l)
{
  return YP_md_wheel_torque(&spur, r, l);
}

int YP_set_wheel_vel(double r, double l)
{
  return YP_md_set_wheel_vel(&spur, r, l);
}

int YP_set_wheel_accel(double r, double l)
{
  return YP_md_set_wheel_accel(&spur, r, l);
}

int YP_wheel_ang(double r, double l)
{
  return YP_md_wheel_ang(&spur, r, l);
}

int YP_joint_torque(int id, double t)
{
  return YP_md_joint_torque(&spur, id, t);
}

int YP_joint_vel(int id, double v)
{
  return YP_md_joint_vel(&spur, id, v);
}

int YP_joint_ang(int id, double a)
{
  return YP_md_joint_ang(&spur, id, a);
}

int YP_joint_ang_vel(int id, double a, double v)
{
  return YP_md_joint_ang_vel(&spur, id, a, v);
}

int YP_set_joint_accel(int id, double a)
{
  return YP_md_set_joint_accel(&spur, id, a);
}

int YP_set_joint_vel(int id, double v)
{
  return YP_md_set_joint_vel(&spur, id, v);
}

double YP_get_joint_vel(int id, double *v)
{
  return YP_md_get_joint_vel(&spur, id, v);
}

double YP_get_joint_vref(int id, double *v)
{
  return YP_md_get_joint_vref(&spur, id, v);
}

double YP_get_joint_ang(int id, double *a)
{
  return YP_md_get_joint_ang(&spur, id, a);
}

double YP_get_joint_torque(int id, double *w)
{
  return YP_md_get_joint_torque(&spur, id, w);
}

void YP_request_device_dump(int id, int block)
{
  YP_md_request_device_dump(&spur, id, block);
}

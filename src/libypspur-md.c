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
 * YP-Spurとの通信ライブラリ複数デバイス対応版 Communication Library for YP-Spur
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
#include <ypspur-md.h>
#include <param.h>

/* エラー確認 */
int YP_md_get_error_state(YPSpur* spur)
{
  return spur->dev.connection_error;
}

double YP_md_get_device_error_state(YPSpur* spur, int id, int* error_state)
{
  YPSpur_msg msg;
  int len;
  double time;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_GET_ERROR_STATE;
  msg.data[0] = id;
  msg.cs = 0;
  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  /* 指定のコマンド受け取り */
  len = spur->dev.recv(&spur->dev, &msg);
  if (len < 0)
  {
    /* receive error */
    spur->connection_error = 1;
    return -1;
  }

  *error_state = (int)msg.data[0];
  time = msg.data[1];
  return time;
}

/* coordinatorとのメッセージ通信を開始する */
int YPSpur_md_initex(YPSpur* spur, int msq_key)
{
  /* メッセージ・キューのオープン */
  if (ipcmd_open_msq(&spur->dev, msq_key, 0) < 0)
  {
    return -1;
  }

  spur->pid = spur->dev.pid;

  return 1;
}

int YPSpur_md_init_socket(YPSpur* spur, char* ip, int port)
{
  /* メッセージ・キューのオープン */
  if (ipcmd_open_tcp(&spur->dev, ip, port) < 0)
  {
    return -1;
  }

  spur->pid = spur->dev.pid;

  return 1;
}

/* coordinatorとのメッセージ通信を開始する */
int YPSpur_md_init(YPSpur* spur)
{
  return YPSpur_md_initex(spur, YPSPUR_MSQ_KEY);
}

/* 直線追従 */
int YPSpur_md_line(YPSpur* spur, int cs, double x, double y, double theta)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_LINE;
  msg.data[0] = x;
  msg.data[1] = y;
  msg.data[2] = theta;
  msg.cs = cs;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }
  return 1;
}

/* 直線追従 */
int YPSpur_md_stop_line(YPSpur* spur, int cs, double x, double y, double theta)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_STOP_LINE;
  msg.data[0] = x;
  msg.data[1] = y;
  msg.data[2] = theta;
  msg.cs = cs;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }
  return 1;
}

/* 円弧追従 */
int YPSpur_md_circle(YPSpur* spur, int cs, double x, double y, double r)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_CIRCLE;
  msg.data[0] = x;
  msg.data[1] = y;
  msg.data[2] = r;
  msg.cs = cs;
  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

/* 旋回 */
int YPSpur_md_spin(YPSpur* spur, int cs, double theta)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_SPIN;
  msg.data[0] = theta;
  msg.cs = cs;
  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

/* 方位 */
int YPSpur_md_orient(YPSpur* spur, int cs, double theta)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_ORIENT;
  msg.data[0] = theta;
  msg.cs = cs;
  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

/* 急ブレーキ */
int YPSpur_md_stop(YPSpur* spur)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_STOP;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

/* 緊急停止 */
int YPSpur_md_freeze(YPSpur* spur)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_FREEZE;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

/* 緊急停止解除 */
int YPSpur_md_unfreeze(YPSpur* spur)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_UNFREEZE;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

/* ソフトウェア補助フリーモード */
int YPSpur_md_free(YPSpur* spur)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_FREE;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

/* 制御なしフリーモード */
int YP_md_openfree(YPSpur* spur)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_OPENFREE;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

/* 位置指定 */
int YPSpur_md_set_pos(YPSpur* spur, int cs, double x, double y, double theta)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_SET_POS;
  msg.data[0] = x;
  msg.data[1] = y;
  msg.data[2] = theta;
  msg.cs = cs;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }
  return 1;
}

/* 位置指定 */
int YPSpur_md_adjust_pos(YPSpur* spur, int cs, double x, double y, double theta)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_ADJUST;
  msg.data[0] = x;
  msg.data[1] = y;
  msg.data[2] = theta;
  msg.cs = cs;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }
  return 1;
}

/* 速度指定 */
int YPSpur_md_set_vel(YPSpur* spur, double v)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_SET_VEL;
  msg.data[0] = v;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

/* 角速度指定 */
int YPSpur_md_set_angvel(YPSpur* spur, double w)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_SET_ANGVEL;
  msg.data[0] = w;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

/* 速度指定 */
int YPSpur_md_set_accel(YPSpur* spur, double dv)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_SET_ACCEL;
  msg.data[0] = dv;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

/* 角速度指定 */
int YPSpur_md_set_angaccel(YPSpur* spur, double dw)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_SET_ANGACCEL;
  msg.data[0] = dw;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

/* 位置取得 */
double YPSpur_md_get_pos(YPSpur* spur, int cs, double* x, double* y, double* theta)
{
  YPSpur_msg msg;
  int len;
  double time;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_GET_POS;
  msg.cs = cs;
  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  /* 指定のコマンド受け取り */
  len = spur->dev.recv(&spur->dev, &msg);
  if (len < 0)
  {
    /* receive error */
    spur->connection_error = 1;
    return -1;
  }

  *x = msg.data[0];
  *y = msg.data[1];
  *theta = msg.data[2];
  time = msg.data[3];
  return time;
}

/* 速度取得 */
double YPSpur_md_get_vel(YPSpur* spur, double* v, double* w)
{
  YPSpur_msg msg;
  int len;
  double time;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_GET_VEL;
  msg.cs = 0;
  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  /* 指定のコマンド受け取り */
  len = spur->dev.recv(&spur->dev, &msg);
  if (len < 0)
  {
    /* receive error */
    spur->connection_error = 1;
    return -1;
  }

  *v = msg.data[0];
  *w = msg.data[1];
  time = msg.data[2];
  return time;
}

/* 速度取得 */
double YP_md_get_vref(YPSpur* spur, double* v, double* w)
{
  YPSpur_msg msg;
  int len;
  double time;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_GET_VREF;
  msg.cs = 0;
  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  /* 指定のコマンド受け取り */
  len = spur->dev.recv(&spur->dev, &msg);
  if (len < 0)
  {
    /* receive error */
    spur->connection_error = 1;
    return -1;
  }

  *v = msg.data[0];
  *w = msg.data[1];
  time = msg.data[2];
  return time;
}

/* 速度取得 */
double YP_md_get_wheel_vref(YPSpur* spur, double* wr, double* wl)
{
  YPSpur_msg msg;
  int len;
  double time;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_GET_WHEEL_VREF;
  msg.cs = 0;
  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  /* 指定のコマンド受け取り */
  len = spur->dev.recv(&spur->dev, &msg);
  if (len < 0)
  {
    /* receive error */
    spur->connection_error = 1;
    return -1;
  }

  *wr = msg.data[0];
  *wl = msg.data[1];
  time = msg.data[2];
  return time;
}

/* 速度取得 */
double YP_md_get_wheel_vel(YPSpur* spur, double* wr, double* wl)
{
  YPSpur_msg msg;
  int len;
  double time;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_GET_WHEEL_VEL;
  msg.cs = 0;
  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  /* 指定のコマンド受け取り */
  len = spur->dev.recv(&spur->dev, &msg);
  if (len < 0)
  {
    /* receive error */
    spur->connection_error = 1;
    return -1;
  }

  *wr = msg.data[0];
  *wl = msg.data[1];
  time = msg.data[2];
  return time;
}

/* 角度取得 */
double YP_md_get_wheel_ang(YPSpur* spur, double* theta_r, double* theta_l)
{
  YPSpur_msg msg;
  int len;
  double time;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_GET_WHEEL_ANG;
  msg.cs = 0;
  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  /* 指定のコマンド受け取り */
  len = spur->dev.recv(&spur->dev, &msg);
  if (len < 0)
  {
    /* receive error */
    spur->connection_error = 1;
    return -1;
  }

  *theta_r = msg.data[0];
  *theta_l = msg.data[1];
  time = msg.data[2];
  return time;
}

/* トルク取得 */
double YP_md_get_wheel_torque(YPSpur* spur, double* torque_r, double* torque_l)
{
  YPSpur_msg msg;
  int len;
  double time;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_GET_WHEEL_TORQUE;
  msg.cs = 0;
  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  /* 指定のコマンド受け取り */
  len = spur->dev.recv(&spur->dev, &msg);
  if (len < 0)
  {
    /* receive error */
    spur->connection_error = 1;
    return -1;
  }

  *torque_r = msg.data[0];
  *torque_l = msg.data[1];
  time = msg.data[2];
  return time;
}

/* 力取得 */
double YPSpur_md_get_force(YPSpur* spur, double* trans, double* angular)
{
  YPSpur_msg msg;
  int len;
  double time;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_GET_FORCE;
  msg.cs = 0;
  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  /* 指定のコマンド受け取り */
  len = spur->dev.recv(&spur->dev, &msg);
  if (len < 0)
  {
    /* receive error */
    spur->connection_error = 1;
    return -1;
  }

  *trans = msg.data[0];
  *angular = msg.data[1];
  time = msg.data[2];
  return time;
}

/* 緊急停止状態取得 */
int YPSpur_md_isfreeze(YPSpur* spur)
{
  YPSpur_msg msg;
  int len;
  int ret;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_ISFREEZE;
  msg.cs = 0;
  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  /* 指定のコマンド受け取り */
  len = spur->dev.recv(&spur->dev, &msg);
  if (len < 0)
  {
    /* receive error */
    spur->connection_error = 1;
    return -1;
  }

  ret = (int)msg.data[0];
  return ret;
}

/* 直接速度入力 */
int YPSpur_md_vel(YPSpur* spur, double v, double w)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_VEL;
  msg.data[0] = v;
  msg.data[1] = w;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

/* 内部パラメータの変更 */
int YP_md_set_parameter(YPSpur* spur, int param_id, double value)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_PARAM_SET;
  msg.cs = param_id;
  msg.data[0] = value;
  msg.data[1] = value;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

/* 内部パラメータの変更 */
int YP_md_set_parameter_array(YPSpur* spur, int param_id, double* value)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_PARAM_SET;
  msg.cs = param_id;
  msg.data[0] = value[0];
  msg.data[1] = value[1];

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

/* 内部パラメータの取得 */
int YP_md_get_parameter(YPSpur* spur, int param_id, double* value)
{
  YPSpur_msg msg;
  int len;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_PARAM_GET;
  msg.cs = param_id;
  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  /* 指定のコマンド受け取り */
  len = spur->dev.recv(&spur->dev, &msg);
  if (len < 0)
  {
    /* receive error */
    spur->connection_error = 1;
    return -1;
  }

  *value = msg.data[0];
  return msg.cs;
}

int YP_md_get_parameter_array(YPSpur* spur, int param_id, double* value)
{
  YPSpur_msg msg;
  int len;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_PARAM_GET;
  msg.cs = param_id;
  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  /* 指定のコマンド受け取り */
  len = spur->dev.recv(&spur->dev, &msg);
  if (len < 0)
  {
    /* receive error */
    spur->connection_error = 1;
    return -1;
  }

  value[0] = msg.data[0];
  value[1] = msg.data[1];
  return msg.cs;
}

/* 内部状態の変更 */
int YP_md_set_control_state(YPSpur* spur, int control_id, int state)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_PARAM_STATE;
  msg.cs = control_id;
  msg.data[0] = state;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

/* 重力補償用地面の傾き指定 */
int YPSpur_md_tilt(YPSpur* spur, int cs, double dir, double tilt)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_SET_TILT;
  msg.data[0] = dir;
  msg.data[1] = tilt;
  msg.cs = cs;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

/* 位置判定 */
int YPSpur_md_near_pos(YPSpur* spur, int cs, double x, double y, double r)
{
  YPSpur_msg msg;
  int len;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_NEAR_POS;
  msg.data[0] = x;
  msg.data[1] = y;
  msg.data[2] = r;
  msg.cs = cs;
  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  /* 指定のコマンド受け取り */
  len = spur->dev.recv(&spur->dev, &msg);
  if (len < 0)
  {
    /* receive error */
    spur->connection_error = 1;
    return -1;
  }

  return msg.cs;
}

/* 角度判定 */
int YPSpur_md_near_ang(YPSpur* spur, int cs, double th, double d)
{
  YPSpur_msg msg;
  int len;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_NEAR_ANG;
  msg.data[0] = th;
  msg.data[1] = d;
  msg.cs = cs;
  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  /* 指定のコマンド受け取り */
  len = spur->dev.recv(&spur->dev, &msg);
  if (len < 0)
  {
    /* receive error */
    spur->connection_error = 1;
    return -1;
  }

  return msg.cs;
}

/* 領域判定 */
int YPSpur_md_over_line(YPSpur* spur, int cs, double x, double y, double theta)
{
  YPSpur_msg msg;
  int len;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_OVER_LINE;
  msg.data[0] = x;
  msg.data[1] = y;
  msg.data[2] = theta;
  msg.cs = cs;
  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  /* 指定のコマンド受け取り */
  len = spur->dev.recv(&spur->dev, &msg);
  if (len < 0)
  {
    /* receive error */
    spur->connection_error = 1;
    return -1;
  }

  return msg.cs;
}

/* アナログ値取得 */
int YP_md_get_ad_value(YPSpur* spur, int num)
{
  YPSpur_msg msg;
  int len;
  int ret;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_GETAD;
  msg.cs = 0;
  msg.data[0] = (double)num;
  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  /* 指定のコマンド受け取り */
  len = spur->dev.recv(&spur->dev, &msg);
  if (len < 0)
  {
    /* receive error */
    spur->connection_error = 1;
    return -1;
  }

  ret = (int)msg.data[0];
  return ret;
}

int YP_md_set_io_dir(YPSpur* spur, unsigned char dir)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_SETIODIR;
  msg.data[0] = dir;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

int YP_md_set_io_data(YPSpur* spur, unsigned char data)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_SETIODATA;
  msg.data[0] = data;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

/* 直接タイヤ回転速度入力 */
int YP_md_wheel_vel(YPSpur* spur, double r, double l)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_WHEEL_VEL;
  msg.data[0] = r;
  msg.data[1] = l;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

int YP_md_wheel_torque(YPSpur* spur, double r, double l)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_WHEEL_TORQUE;
  msg.data[0] = r;
  msg.data[1] = l;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

int YP_md_set_wheel_vel(YPSpur* spur, double r, double l)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_SET_WHEEL_VEL;
  msg.data[0] = r;
  msg.data[1] = l;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

int YP_md_set_wheel_accel(YPSpur* spur, double r, double l)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_SET_WHEEL_ACCEL;
  msg.data[0] = r;
  msg.data[1] = l;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

int YP_md_wheel_ang(YPSpur* spur, double r, double l)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_WHEEL_ANGLE;
  msg.data[0] = r;
  msg.data[1] = l;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

int YP_md_joint_torque(YPSpur* spur, int id, double t)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_JOINT_TORQUE;
  msg.cs = id;
  msg.data[0] = t;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

int YP_md_joint_vel(YPSpur* spur, int id, double v)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_JOINT_VEL;
  msg.cs = id;
  msg.data[0] = v;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

int YP_md_joint_ang(YPSpur* spur, int id, double a)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_JOINT_ANG;
  msg.cs = id;
  msg.data[0] = a;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

int YP_md_joint_ang_vel(YPSpur* spur, int id, double a, double v)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_JOINT_ANG_VEL;
  msg.cs = id;
  msg.data[0] = a;
  msg.data[1] = v;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

int YP_md_set_joint_accel(YPSpur* spur, int id, double a)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_SET_JOINT_ACCEL;
  msg.cs = id;
  msg.data[0] = a;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

int YP_md_set_joint_vel(YPSpur* spur, int id, double v)
{
  YPSpur_msg msg;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_SET_JOINT_VEL;
  msg.cs = id;
  msg.data[0] = v;

  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  return 1;
}

double YP_md_get_joint_vel(YPSpur* spur, int id, double* v)
{
  YPSpur_msg msg;
  int len;
  double time;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_GET_JOINT_VEL;
  msg.cs = id;
  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  /* 指定のコマンド受け取り */
  len = spur->dev.recv(&spur->dev, &msg);
  if (len < 0)
  {
    /* receive error */
    spur->connection_error = 1;
    return -1;
  }

  *v = msg.data[0];
  time = msg.data[1];
  return time;
}

double YP_md_get_joint_vref(YPSpur* spur, int id, double* v)
{
  YPSpur_msg msg;
  int len;
  double time;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_GET_JOINT_VREF;
  msg.cs = id;
  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  /* 指定のコマンド受け取り */
  len = spur->dev.recv(&spur->dev, &msg);
  if (len < 0)
  {
    /* receive error */
    spur->connection_error = 1;
    return -1;
  }

  *v = msg.data[0];
  time = msg.data[1];
  return time;
}

double YP_md_get_joint_ang(YPSpur* spur, int id, double* a)
{
  YPSpur_msg msg;
  int len;
  double time;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_GET_JOINT_ANG;
  msg.cs = id;
  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  /* 指定のコマンド受け取り */
  len = spur->dev.recv(&spur->dev, &msg);
  if (len < 0)
  {
    /* receive error */
    spur->connection_error = 1;
    return -1;
  }

  *a = msg.data[0];
  time = msg.data[1];
  return time;
}

double YP_md_get_joint_torque(YPSpur* spur, int id, double* t)
{
  YPSpur_msg msg;
  int len;
  double time;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_GET_JOINT_TORQUE;
  msg.cs = id;
  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
    return -1;
  }

  /* 指定のコマンド受け取り */
  len = spur->dev.recv(&spur->dev, &msg);
  if (len < 0)
  {
    /* receive error */
    spur->connection_error = 1;
    return -1;
  }

  *t = msg.data[0];
  time = msg.data[1];
  return time;
}

void YP_md_request_device_dump(YPSpur* spur, int id, int block)
{
  YPSpur_msg msg;
  int len;

  msg.msg_type = YPSPUR_MSG_CMD;
  msg.pid = spur->pid;
  msg.type = YPSPUR_REQUEST_DEVICE_DUMP;
  msg.data[0] = id;
  msg.data[1] = block;
  msg.cs = 0;
  if (spur->dev.send(&spur->dev, &msg) < 0)
  {
    /* error */
    spur->connection_error = 1;
  }
}

ParamOutputLv output_lv(void)
{
  return OUTPUT_LV_INFO;
}

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
#include <time.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif  // HAVE_CONFIG_H

/* ボディパラメータ */
#include <shvel-param.h>

/* yp-spur用 */
#include <serial.h>
#include <param.h>
#include <control.h>
#include <command.h>
#include <ipcommunication.h>
#include <yprintf.h>

/* ライブラリ用 */
#include <ypspur.h>
#include <cartesian2d.h>

#include <pthread.h>

SpurUserParams g_spur;

SpurUserParamsPtr get_spur_user_param_ptr()
{
  return &g_spur;
}

void command_loop_cleanup(void *data)
{
  struct ipcmd_t *ipcmd;

  ipcmd = data;
  yprintf(OUTPUT_LV_INFO, "Command analyzer stopped.\n");

  ipcmd_close(ipcmd);
}

void init_spur_command(void)
{
  g_spur.x = 0;
  g_spur.y = 0;
  g_spur.theta = 0;
  g_spur.v = 0;
  g_spur.w = 0;
  g_spur.radius = 0;
  g_spur.tilt = 0;
  g_spur.dir = 0;
  g_spur.run_mode = RUN_STOP;
  g_spur.before_run_mode = -10000;
  g_spur.freeze = 0;
  g_spur.before_freeze = 0;
  g_spur.run_mode_cnt = 0;
  pthread_mutex_init(&g_spur.mutex, NULL);
}

/* コマンド受信 */
void command(void)
{
  YPSpur_msg msg, res_msg;
  int len;
  struct ipcmd_t ipcmd;
  char param_name[YP_PARAM_NUM][30] = YP_PARAM_NAME;
  ParametersPtr param;

  param = get_param_ptr();

  /* initialize message queue */
  if (option(OPTION_SOCKET))
  {
    len = ipcmd_open_tcp(&ipcmd, NULL, get_param_ptr()->port);
  }
  else
  {
    len = ipcmd_open_msq(&ipcmd, get_param_ptr()->msq_key, 1);
  }
  if (len < 0)
  {
    /* queue_error */
    yprintf(OUTPUT_LV_ERROR, "Error: Can't initialize message queue.\n");
    return;
  }

  // メッセージキューを空にする
  ipcmd.flush(&ipcmd);

  yprintf(OUTPUT_LV_INFO, "Command analyzer started.\n");
  pthread_cleanup_push(command_loop_cleanup, &ipcmd);

  while (1)
  {
    /* 1コマンド取得 */
    len = ipcmd.recv(&ipcmd, &msg);
    if (len < YPSPUR_MSG_SIZE)
    {
      yprintf(OUTPUT_LV_ERROR, "Error: Invalid command received\n");
      continue;
    }

    pthread_mutex_lock(&g_spur.mutex);
    /* コマンド解析・返信 */
    switch (msg.type)
    {
      /*--------command_set.c--------*/
      case YPSPUR_SET_POS:
        set_pos_com(msg.cs, msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: set pos %f %f %f\n", msg.data[0], msg.data[1], msg.data[2]);
        break;
      case YPSPUR_SET_GL_GL:
        set_GL_on_GL_com(msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: GL on GL  %f %f %f\n", msg.data[0], msg.data[1], msg.data[2]);
        break;
      case YPSPUR_ADJUST:
        set_adjust_com(msg.cs, msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: adjust  %f %f %f\n", msg.data[0], msg.data[1], msg.data[2]);
        break;
      case YPSPUR_SET_VEL:
        set_vel_com(msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: vel %f\n", g_spur.v);
        break;
      case YPSPUR_SET_ANGVEL:
        set_ang_vel_com(msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: w %f\n", g_spur.w);
        break;
      case YPSPUR_SET_ACCEL:
        set_accel_com(msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: dvel %f\n", g_spur.dv);
        break;
      case YPSPUR_SET_ANGACCEL:
        set_ang_accel_com(msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: dw %f\n", g_spur.dw);
        break;
      case YPSPUR_SET_TILT:
        set_tilt_com(msg.cs, msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: tilt %f %f\n", g_spur.dir, g_spur.tilt);
        break;

      /*--------command_run.c----------*/
      case YPSPUR_LINE:
        line_com(msg.cs, msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: line %f %f %f\n", g_spur.x, g_spur.y, g_spur.theta);
        break;
      case YPSPUR_STOP_LINE:
        stop_line_com(msg.cs, msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: stop_line %f %f %f\n", g_spur.x, g_spur.y, g_spur.theta);
        break;
      case YPSPUR_CIRCLE:
        circle_com(msg.cs, msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: circle %f %f %f\n", g_spur.x, g_spur.y, g_spur.radius);
        break;
      case YPSPUR_SPIN:
        spin_com(msg.cs, msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: spin %f\n", g_spur.theta);
        break;
      case YPSPUR_ORIENT:
        orient_com(msg.cs, msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: orient %f\n", g_spur.theta);
        break;
      case YPSPUR_STOP:
        stop_com(msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: stop\n");
        break;
      case YPSPUR_OPENFREE:
        openfree_com(msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: openfree\n");
        break;
      case YPSPUR_FREE:
        free_com(msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: free\n");
        break;
      case YPSPUR_FREEZE:
        g_spur.freeze = 1;
        yprintf(OUTPUT_LV_DEBUG, "Command: freeze\n");
        break;
      case YPSPUR_UNFREEZE:
        g_spur.freeze = 0;
        yprintf(OUTPUT_LV_DEBUG, "Command: unfreeze\n");
        break;
      case YPSPUR_ISFREEZE:
        res_msg.data[0] = g_spur.freeze;
        message_return(&ipcmd, msg.pid, &res_msg);
        yprintf(OUTPUT_LV_DEBUG, "Command: isfreeze %d\n", g_spur.freeze);
        break;
      case YPSPUR_VEL:
        vel_com(msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: vel %f %f\n", g_spur.vref, g_spur.wref);
        break;
      case YPSPUR_WHEEL_VEL:
        wheel_vel_com(msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: wheel_vel %f %f\n", g_spur.wvelref[0], g_spur.wvelref[1]);
        break;
      case YPSPUR_WHEEL_TORQUE:
        set_torque_com(msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: set_torque %f %f\n", msg.data[0], msg.data[1]);
        break;
      case YPSPUR_SET_WHEEL_VEL:
        set_wheel_vel_com(msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: set_wheel_vel %f %f\n", g_spur.wheel_vel[0], g_spur.wheel_vel[1]);
        break;
      case YPSPUR_SET_WHEEL_ACCEL:
        set_wheel_accel_com(msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: set_wheel_accel %f %f\n", g_spur.wheel_accel[0], g_spur.wheel_accel[1]);
        break;
      case YPSPUR_WHEEL_ANGLE:
        wheel_angle_com(msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: wheel_angle %f %f\n", g_spur.wheel_angle[0], g_spur.wheel_angle[1]);
        break;

      /*----------command_get.c------------------*/
      case YPSPUR_GET_POS:
        get_pos_com(msg.cs, msg.data, res_msg.data, &g_spur);
        message_return(&ipcmd, msg.pid, &res_msg);
        yprintf(OUTPUT_LV_DEBUG, "Command: get %f %f %f\n", res_msg.data[0], res_msg.data[1], res_msg.data[2]);
        break;
      case YPSPUR_GET_VEL:
        get_vel_com(msg.cs, msg.data, res_msg.data, &g_spur);
        message_return(&ipcmd, msg.pid, &res_msg);
        yprintf(OUTPUT_LV_DEBUG, "Command: getvel %f %f\n", res_msg.data[0], res_msg.data[1]);
        break;
      case YPSPUR_GET_VREF:
        get_vref_com(msg.cs, msg.data, res_msg.data, &g_spur);
        message_return(&ipcmd, msg.pid, &res_msg);
        yprintf(OUTPUT_LV_DEBUG, "Command: getvref %f %f\n", res_msg.data[0], res_msg.data[1]);
        break;
      case YPSPUR_GET_WHEEL_VREF:
        get_wheel_vref_com(msg.cs, msg.data, res_msg.data, &g_spur);
        message_return(&ipcmd, msg.pid, &res_msg);
        yprintf(OUTPUT_LV_DEBUG, "Command: getwheelvref %f %f\n", res_msg.data[0], res_msg.data[1]);
        break;
      case YPSPUR_GET_FORCE:
        get_force_com(msg.cs, msg.data, res_msg.data, &g_spur);
        message_return(&ipcmd, msg.pid, &res_msg);
        yprintf(OUTPUT_LV_DEBUG, "Command: getforce %f %f\n", res_msg.data[0], res_msg.data[1]);
        break;
      case YPSPUR_GET_WHEEL_TORQUE:
        get_wheel_torque_com(msg.cs, msg.data, res_msg.data, &g_spur);
        message_return(&ipcmd, msg.pid, &res_msg);
        yprintf(OUTPUT_LV_DEBUG, "Command: getwheeltorque %f %f\n", res_msg.data[0], res_msg.data[1]);
        break;
      case YPSPUR_GET_WHEEL_VEL:
        get_wheel_vel_com(msg.data, res_msg.data, &g_spur);
        message_return(&ipcmd, msg.pid, &res_msg);
        yprintf(OUTPUT_LV_DEBUG, "Command: get wheelvel %f %f\n", res_msg.data[0], res_msg.data[1]);
        break;
      case YPSPUR_GET_WHEEL_ANG:
        get_wheel_ang_com(msg.data, res_msg.data, &g_spur);
        message_return(&ipcmd, msg.pid, &res_msg);
        yprintf(OUTPUT_LV_DEBUG, "Command: get wheelang %f %f\n", res_msg.data[0], res_msg.data[1]);
        break;
      case YPSPUR_NEAR_POS:
        res_msg.cs = near_pos_com(msg.cs, msg.data, res_msg.data, &g_spur);
        message_return(&ipcmd, msg.pid, &res_msg);
        yprintf(OUTPUT_LV_DEBUG, "Command: near pos ( dist = %f )\n", res_msg.data[0]);
        break;
      case YPSPUR_NEAR_ANG:
        res_msg.cs = near_ang_com(msg.cs, msg.data, res_msg.data, &g_spur);
        message_return(&ipcmd, msg.pid, &res_msg);
        yprintf(OUTPUT_LV_DEBUG, "Command: near ang ( dist = %f )\n", res_msg.data[0]);
        break;
      case YPSPUR_OVER_LINE:
        res_msg.cs = over_line_com(msg.cs, msg.data, res_msg.data, &g_spur);
        message_return(&ipcmd, msg.pid, &res_msg);
        yprintf(OUTPUT_LV_DEBUG, "Command: over line ( dist = %f )\n", res_msg.data[0]);
        break;

      /*-------------command_param.c---------------*/
      case YPSPUR_PARAM_SET:
        param_set_com(msg.cs, msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: param_set %s %f\n", param_name[msg.cs], msg.data[0]);
        break;

      case YPSPUR_PARAM_GET:
        res_msg.cs = param_get_com(msg.cs, res_msg.data, &g_spur);
        message_return(&ipcmd, msg.pid, &res_msg);
        yprintf(OUTPUT_LV_DEBUG, "Command: param_get %s %f\n", param_name[msg.cs], msg.data[0]);
        break;

      case YPSPUR_PARAM_STATE:
        param_state_com(msg.cs, msg.data, &g_spur);
        break;

      /*-------------command_aux.c---------------*/
      case YPSPUR_GETAD:
        get_ad_com(msg.data, res_msg.data);
        message_return(&ipcmd, msg.pid, &res_msg);
        yprintf(OUTPUT_LV_DEBUG, "Command: get A/D%d value %d\n", (int)msg.data[0], (int)res_msg.data[0]);
        break;

      case YPSPUR_SETIODIR:
        set_io_dir_com(msg.data, res_msg.data);
        yprintf(OUTPUT_LV_DEBUG, "Command: set IO dir %d\n", (int)msg.data[0]);
        break;

      case YPSPUR_SETIODATA:
        set_io_data_com(msg.data, res_msg.data);
        yprintf(OUTPUT_LV_DEBUG, "Command: set IO data %d\n", (int)msg.data[0]);
        break;

      case YPSPUR_GET_ERROR_STATE:
        get_error_state_com(msg.data, res_msg.data);
        message_return(&ipcmd, msg.pid, &res_msg);
        yprintf(OUTPUT_LV_DEBUG, "Command: get error state %d (id: %d)\n", (int)msg.data[0], (int)res_msg.data[0]);
        break;

      case YPSPUR_REQUEST_DEVICE_DUMP:
        dump_device_com(msg.data, res_msg.data);
        yprintf(OUTPUT_LV_DEBUG, "Command: dump_device %d %d\n", (int)msg.data[0], (int)msg.data[1]);
        break;

      /*-------------command_joint.c---------------*/
      case YPSPUR_JOINT_TORQUE:
        joint_torque_com(msg.cs, msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: joint %d torque %f\n", msg.cs, msg.data[0]);
        break;

      case YPSPUR_JOINT_VEL:
        joint_vel_com(msg.cs, msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: joint %d vel %f\n", msg.cs, msg.data[0]);
        break;

      case YPSPUR_JOINT_ANG:
        joint_ang_com(msg.cs, msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: joint %d ang %f\n", msg.cs, msg.data[0]);
        break;

      case YPSPUR_JOINT_ANG_VEL:
        joint_ang_vel_com(msg.cs, msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: joint %d ang %f vel %f\n", msg.cs, msg.data[0], msg.data[1]);
        break;

      case YPSPUR_SET_JOINT_ACCEL:
        set_joint_accel_com(msg.cs, msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: set joint %d accel %f\n", msg.cs, msg.data[0]);
        break;

      case YPSPUR_SET_JOINT_VEL:
        set_joint_vel_com(msg.cs, msg.data, &g_spur);
        yprintf(OUTPUT_LV_DEBUG, "Command: set joint %d vel %f\n", msg.cs, msg.data[0]);
        break;

      case YPSPUR_GET_JOINT_VEL:
        get_joint_vel_com(msg.cs, res_msg.data, &g_spur);
        message_return(&ipcmd, msg.pid, &res_msg);
        yprintf(OUTPUT_LV_DEBUG, "Command: get joint %d vel %f\n", msg.cs, res_msg.data[0]);
        break;

      case YPSPUR_GET_JOINT_VREF:
        get_joint_vref_com(msg.cs, res_msg.data, &g_spur);
        message_return(&ipcmd, msg.pid, &res_msg);
        yprintf(OUTPUT_LV_DEBUG, "Command: get joint %d vref %f\n", msg.cs, res_msg.data[0]);
        break;

      case YPSPUR_GET_JOINT_ANG:
        get_joint_ang_com(msg.cs, res_msg.data, &g_spur);
        message_return(&ipcmd, msg.pid, &res_msg);
        yprintf(OUTPUT_LV_DEBUG, "Command: get joint %d ang %f\n", msg.cs, res_msg.data[0]);
        break;

      case YPSPUR_GET_JOINT_TORQUE:
        get_joint_torque_com(msg.cs, res_msg.data, &g_spur);
        message_return(&ipcmd, msg.pid, &res_msg);
        yprintf(OUTPUT_LV_DEBUG, "Command: get joint %d torque %f\n", msg.cs, res_msg.data[0]);
        break;

      default:
        yprintf(OUTPUT_LV_WARNING, "Command: unknown\n");
        break;
    }

    /* 走行モードに変化があったか */
    if (g_spur.run_mode != g_spur.before_run_mode || g_spur.before_freeze != g_spur.freeze)
    {
      if (!g_spur.freeze)
      {
        if (g_spur.run_mode == RUN_FREE || g_spur.run_mode == RUN_WHEEL_TORQUE)
        { /* フリーになった */
          int i;
          for (i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i++)
          {
            if (!param->motor_enable[i])
              continue;
            if (p(YP_PARAM_VEHICLE_CONTROL, i) > 0)
              g_spur.wheel_mode[i] = MOTOR_CONTROL_FREE;
          }
          yprintf(OUTPUT_LV_DEBUG, "Mode: free\n");
        }
        else if (g_spur.run_mode == RUN_OPENFREE)
        {
          int i;
          for (i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i++)
          {
            if (!param->motor_enable[i])
              continue;
            if (p(YP_PARAM_VEHICLE_CONTROL, i) > 0)
              g_spur.wheel_mode[i] = MOTOR_CONTROL_OPENFREE;
          }
          yprintf(OUTPUT_LV_DEBUG, "Mode: openfree\n");
        }
        else
        {
          int i;
          for (i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i++)
          {
            if (!param->motor_enable[i])
              continue;
            if (p(YP_PARAM_VEHICLE_CONTROL, i) > 0)
              g_spur.wheel_mode[i] = MOTOR_CONTROL_VEHICLE;
          }
          yprintf(OUTPUT_LV_DEBUG, "Mode: servo %d\n", g_spur.run_mode);
        }
      }
      else
      {
        yprintf(OUTPUT_LV_DEBUG, "Mode: freeze\n");
      }
    }
    g_spur.before_run_mode = g_spur.run_mode;
    g_spur.before_freeze = g_spur.freeze;

    pthread_mutex_unlock(&g_spur.mutex);
    // スレッドの停止要求チェック
    pthread_testcancel();
  }

  pthread_cleanup_pop(1);
}

/* メッセージを返す */
void message_return(struct ipcmd_t *ipcmd, long retpid, YPSpur_msg *res_msg)
{
  res_msg->type = 0;
  res_msg->msg_type = retpid;
  res_msg->pid = 0;
  ipcmd->send(ipcmd, res_msg);
}

/* すれっどの初期化 */
void init_command_thread(pthread_t *thread)
{
  if (pthread_create(thread, NULL, (void *)command, NULL) != 0)
  {
    yprintf(OUTPUT_LV_ERROR, "Can't create command thread\n");
  }
}

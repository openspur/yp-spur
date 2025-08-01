// Copyright (c) 2010-2025 The YP-Spur Authors, except where otherwise indicated.
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

#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#include <pthread.h>

#include <ypspur/adinput.h>
#include <ypspur/command.h>
#include <ypspur/control.h>
#include <ypspur/odometry.h>
#include <ypspur/param.h>
#include <ypspur/ping.h>
#include <ypspur/serial.h>
#include <ypspur/signal.h>
#include <ypspur/ssm_spur_handler.h>
#include <ypspur/utility.h>
#include <ypspur/ypparam.h>
#include <ypspur/ypprotocol.h>
#include <ypspur/yprintf.h>
#include <ypspur/ypspur-coordinator.h>

atomic_int g_simulation_exit = 0;

int ypsc_main(int argc, char* argv[])
{
  pthread_t command_thread;
  pthread_t control_thread;
  pthread_t update_thread;
  int command_thread_en;
  int control_thread_en;
  int update_thread_en;
  Ver_t version;
  Param_t driver_param;
  char paramfile[512];
  int quit;

  hook_pre_global();

  g_simulation_exit = 0;

  const int ret = arg_analyze(argc, argv);
  if (option(OPTION_DAEMON))
  {
#if HAVE_FORK
    pid_t pid;

    pid = fork();
    if (pid < 0)
    {
      return -1;
    }
    else if (pid != 0)
    {
      return 0;
    }
    setsid();
    if (chdir("/") < 0)
    {
      yprintf(OUTPUT_LV_ERROR, "Failed to chdir.\n");
      return EXIT_FAILURE;
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
#else
    yprintf(OUTPUT_LV_ERROR, "Daemon mode is not supported in your system.\n");
    return EXIT_FAILURE;
#endif  // HAVE_FORK
  }
  if (option(OPTION_SHOW_HELP))
  {
    arg_help(argc, argv);
    return EXIT_SUCCESS;
  }
  if (option(OPTION_SHOW_LONGHELP))
  {
    arg_longhelp(argc, argv);
    return EXIT_SUCCESS;
  }
  if (option(OPTION_SHOW_PARAMHELP))
  {
    param_help();
    return EXIT_SUCCESS;
  }
  if (option(OPTION_VERSION))
  {
    fprintf(stderr, "YamabicoProject-Spur\n");
    fprintf(stderr, " Ver. %s\n", PROJECT_VERSION);
    return EXIT_SUCCESS;
  }
  if (!ret)
  {
    // オプション解析に失敗したとき
    return EXIT_FAILURE;
  }

  yprintf(OUTPUT_LV_INFO, "++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  yprintf(OUTPUT_LV_INFO, "YamabicoProject-Spur\n");
  yprintf(OUTPUT_LV_INFO, " Ver. %s\n", PROJECT_VERSION);
  yprintf(OUTPUT_LV_INFO, "++++++++++++++++++++++++++++++++++++++++++++++++++\n");

#ifdef HAVE_SSM
  // SSM初期化
  if (!option(OPTION_WITHOUT_SSM))
    init_ypspurSSM(get_param_ptr()->ssm_id);
#endif  // HAVE_SSM

  // 座標系の初期化、コマンド処理系の初期化
  init_coordinate_systems();
  init_odometry();
  init_spur_command();

  fflush(stderr);

  int* control_thread_status = NULL;
  command_thread_en = 0;
  command_thread_en = 0;
  do
  {
    FILE* temp_paramfile = NULL;
    quit = 0;

    yprintf(OUTPUT_LV_INFO, "Device Information\n");

    // Clear driver params
    driver_param.robot_name[0] = 0;
    driver_param.pwm_resolution[0] = 0;
    strcpy(driver_param.motor_num, "2");

    if (!(option(OPTION_WITHOUT_DEVICE)))
    {
      yprintf(OUTPUT_LV_INFO, " Port    : %s \n", get_param_ptr()->device_name);
      if (!serial_connect(get_param_ptr()->device_name))
      {
        // quit=0;でbreakしたら異常終了と判断される
        break;
      }
      if (!(option(OPTION_DO_NOT_USE_YP)))
      {
        int i;
        int current, age;
        int device_current, device_age;
        sscanf(YP_PROTOCOL_NAME, "YPP:%d:%d", &current, &age);

        yprintf(OUTPUT_LV_INFO, " Checking device information");
        for (i = 0; i < 3; i++)
        {
          yprintf(OUTPUT_LV_INFO, ".");
          // Check protocol version
          if (get_version(&version) == -1)
          {
            continue;
          }
          if (strstr(version.protocol, "YPP:") != version.protocol)
          {
            continue;
          }
          sscanf(version.protocol, "YPP:%d:%d", &device_current, &device_age);
          get_param_ptr()->device_version = device_current;
          get_param_ptr()->device_version_age = device_age;
          if (device_current - device_age > current ||
              device_current < current - age)
          {
            continue;
          }
          break;
        }
        ansi_clear_line(OUTPUT_LV_INFO);
        yprintf(OUTPUT_LV_INFO, " Vender  : %s\n", version.vender);
        yprintf(OUTPUT_LV_INFO, " Product : %s\n", version.product);
        yprintf(OUTPUT_LV_INFO, " Firmware: %s\n", version.firmware);
        yprintf(OUTPUT_LV_INFO, " Protcol : %s\n", version.protocol);
        yprintf(OUTPUT_LV_INFO, " Serialno: %s\n", version.serialno);
        yprintf(OUTPUT_LV_INFO, "++++++++++++++++++++++++++++++++++++++++++++++++++\n");
        if (i == 3)
        {
          yprintf(OUTPUT_LV_ERROR, "Error: Device doesn't have available YP protocol version.\n(Device: %s, coordinator: %s)\n",
                  version.protocol, YP_PROTOCOL_NAME);
          if (option(OPTION_RECONNECT))
          {
            yp_usleep(500000);
            continue;
          }
          break;  // quit=0でbreakしたら異常終了と判断
        }
        if (device_current != current)
        {
          if (device_current < current)
          {
            yprintf(OUTPUT_LV_WARNING, "Recommendation: Device protocol version is not latest.\n");
            yprintf(OUTPUT_LV_WARNING, "Recommendation: Firmware update is recommended.\n");
          }
          else
          {
            yprintf(OUTPUT_LV_WARNING, "Recommendation: ypspur-coordinator protocol version is not latest.\n");
            yprintf(OUTPUT_LV_WARNING, "Recommendation: Software update is recommended.\n");
          }
          yprintf(OUTPUT_LV_WARNING, "++++++++++++++++++++++++++++++++++++++++++++++++++\n");
        }
        if (get_parameter(&driver_param) == -1)
        {
          continue;
        }
        yprintf(OUTPUT_LV_DEBUG, "Driver depending parameters\n");
        yprintf(OUTPUT_LV_DEBUG, " Name          : %s\n", driver_param.robot_name);
        yprintf(OUTPUT_LV_DEBUG, " PWM resolution: %s\n", driver_param.pwm_resolution);
        yprintf(OUTPUT_LV_DEBUG, " Motor number  : %s\n", driver_param.motor_num);
        yprintf(OUTPUT_LV_DEBUG, "++++++++++++++++++++++++++++++++++++++++++++++++++\n");

        if (strlen(driver_param.pwm_resolution) <= 0 ||
            strlen(driver_param.motor_num) <= 0)
        {
          yprintf(OUTPUT_LV_ERROR, "Error: Failed to load driver parameters.\n");
          if (option(OPTION_RECONNECT))
          {
            yp_usleep(500000);
            continue;
          }
          break;
        }
      }
      fflush(stderr);
    }
    else
    {
      yprintf(OUTPUT_LV_INFO, " Port    : n/a (--without-device mode)\n");
    }

    if (option(OPTION_PING))
    {
      return ping();
    }

    if (!(option(OPTION_PARAM_FILE)))
    {
      // パラメータファイルが指定されておらず、ドライバにパラメータが内蔵されている場合
      if (strcmp(driver_param.robot_name, "embedded") == 0)
      {
        char param[2048];

        yprintf(OUTPUT_LV_INFO, "Reading device embedded parameter.\n");
        temp_paramfile = tmpfile();
        if (!temp_paramfile)
        {
          yprintf(OUTPUT_LV_ERROR, "Error: Failed to create temporary file.\n");
          break;
        }
        if (!get_embedded_param(param))
        {
          yprintf(OUTPUT_LV_ERROR, "Error: Failed to read embedded parameters.\n");
          if (option(OPTION_RECONNECT))
          {
            yp_usleep(500000);
            continue;
          }
          break;
        }
        fprintf(temp_paramfile, "%s", param);
        fseek(temp_paramfile, 0L, SEEK_SET);
      }
      // パラメータファイルが指定されておらず、ドライバにロボット名が登録されている場合
      else if (strlen(driver_param.robot_name) > 0 && strcmp(driver_param.robot_name, "unknown") != 0)
      {
        strcpy(get_param_ptr()->parameter_filename, driver_param.robot_name);
        strcat(get_param_ptr()->parameter_filename, ".param");
      }
    }
    if (temp_paramfile)
    {
      yprintf(OUTPUT_LV_DEBUG, "Embedded parameter file\n");
      if (!set_paramptr(temp_paramfile))
      {
        yprintf(OUTPUT_LV_ERROR, "Error: Cannot use embedded parameter.\n");
        break;
      }
    }
    else
    {
      yprintf(OUTPUT_LV_DEBUG, "Parameter file: %s\n", get_param_ptr()->parameter_filename);
      if (!set_param(get_param_ptr()->parameter_filename, paramfile))
      {
        yprintf(OUTPUT_LV_ERROR, "Error: Cannot load parameter file.\n");
        break;
      }
    }
    {
      if (strlen(driver_param.pwm_resolution) > 0)
      {
        for (int i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i++)
        {
          *pp(YP_PARAM_PWM_MAX, i) = atoi(driver_param.pwm_resolution);
        }
      }
      else if (p(YP_PARAM_PWM_MAX, 0) == 0)
      {
        if (option(OPTION_DO_NOT_USE_YP))
        {
          for (int i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i++)
          {
            *pp(YP_PARAM_PWM_MAX, i) = 1000;  // dummy value for simulation
          }
        }
        else
        {
          yprintf(OUTPUT_LV_ERROR, "Error: _PWM_RESOLUTION parameter must be given on --no-yp-protocol mode.\n");
          break;
        }
      }
    }
    yprintf(OUTPUT_LV_DEBUG, "++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");

    if (!(option(OPTION_WITHOUT_DEVICE)))
    {
      if (!(option(OPTION_DO_NOT_USE_YP)))
      {
        // ボーレートの設定
        if (get_param_ptr()->speed)
        {
          yprintf(OUTPUT_LV_INFO, "Setting baudrate to %d baud.\n", get_param_ptr()->speed);
        }
        else
        {
          // 指定されてない場合デフォルトの値
          get_param_ptr()->speed = DEFAULT_BAUDRATE;
        }

        const int ret = set_baudrate(get_param_ptr()->speed);
        if (ret == 0)
        {
          // 設定失敗
          yprintf(OUTPUT_LV_WARNING, "Error: Failed to change baudrate.\n");
          break;  // quit=0でbreakしたら異常終了と判断
        }
        if (ret == 4)
        {
          // ボーレートの設定未対応
          yprintf(OUTPUT_LV_INFO, "Info: Baudrate setting is not supported on this device.\n");
        }
        else
        {
          // 設定成功
          // 正常ならば何もしない
        }

        if (get_param_ptr()->admask)
        {
          yprintf(OUTPUT_LV_INFO, "Setting admask to %x.\n", get_param_ptr()->admask);
          set_admask(get_param_ptr()->admask);
        }

        if (option(OPTION_ENABLE_GET_DIGITAL_IO))
        {
          yprintf(OUTPUT_LV_INFO, "Enabling digital io input.\n");
          set_diomask(1);
        }
      }

      if (!(option(OPTION_PARAM_CONTROL)))
        if (apply_robot_params() < 1)
          break;

      // サーボをかける
      SpurUserParamsPtr spur;
      spur = get_spur_user_param_ptr();
      for (int i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i++)
      {
        spur->wheel_mode[i] = MOTOR_CONTROL_VEL;
        spur->wheel_mode_prev[i] = -1;
      }
    }

    yprintf(OUTPUT_LV_INFO, "YP-Spur coordinator started.\n");

    // スレッド初期化
    if (!option(OPTION_NO_IPC))
    {
      init_command_thread(&command_thread);
      command_thread_en = 1;
    }

    init_control_thread(&control_thread);
    control_thread_en = 1;

    if (option(OPTION_UPDATE_PARAM))
    {
      yprintf(OUTPUT_LV_WARNING, "==================== Warning! ====================\n");
      yprintf(OUTPUT_LV_WARNING, "Running in automatic parameter update mode.\n");
      yprintf(OUTPUT_LV_WARNING, "Keep in mind that the coordinator doesn't stop\n");
      yprintf(OUTPUT_LV_WARNING, "even if the parameter file gets invalid.\n");
      yprintf(OUTPUT_LV_WARNING, "==================================================\n");
      init_param_update_thread(&update_thread, paramfile);
      update_thread_en = 1;
    }
    else
    {
      update_thread_en = 0;
    }

    // オドメトリ受信ループ
#if HAVE_SIGLONGJMP
    if (sigsetjmp(*get_ctrlc_jmp_buf_ptr(), 1) != 0)
    {
      quit = 1;
    }
    else
#elif HAVE_LONGJMP
    if (setjmp(*get_ctrlc_jmp_buf_ptr()) != 0)
    {
      quit = 1;
    }
    else
#endif
    {
      enable_ctrlc_handling(1);
      if (!(option(OPTION_WITHOUT_DEVICE)))
      {
        odometry_receive_loop();
      }
      else
      {
        while (!g_simulation_exit)
        {
          yp_usleep(1000000);
        }
      }
      yprintf(OUTPUT_LV_INFO, "Connection to %s was closed.\n", get_param_ptr()->device_name);
    }
    enable_ctrlc_handling(0);

    // 終了処理
    if (update_thread_en)
    {
      pthread_cancel(update_thread);
      pthread_join(update_thread, NULL);
      update_thread_en = 0;
    }
    if (control_thread_en)
    {
      pthread_cancel(control_thread);
      pthread_join(control_thread, (void**)&control_thread_status);
      control_thread_en = 0;
    }
    if (command_thread_en)
    {
      pthread_cancel(command_thread);
      pthread_join(command_thread, NULL);
      command_thread_en = 0;
    }

    if (option(OPTION_RECONNECT) && quit == 0 && control_thread_status == NULL)
    {
      init_spur_command();
      yp_usleep(500000);
      if (!(option(OPTION_WITHOUT_DEVICE)))
      {
        serial_close();
        while (!serial_tryconnect(get_param_ptr()->device_name))
        {
          yp_usleep(200000);
        }
      }
      yprintf(OUTPUT_LV_INFO, "++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      yp_usleep(500000);
      continue;
    }
    break;
  }
  while (1);

  if (!(option(OPTION_WITHOUT_DEVICE)))
    serial_close();

#ifdef HAVE_SSM
  // SSM終了処理
  if (!option(OPTION_WITHOUT_SSM))
    end_ypspurSSM();
#endif  // HAVE_SSM

  yp_usleep(200000);
  fflush(stderr);

  if (control_thread_status != NULL && control_thread_status != PTHREAD_CANCELED)
  {
    return *control_thread_status;
  }

  if (option(OPTION_WITHOUT_DEVICE))
  {
    g_simulation_exit = 0;
  }

  return (quit ? EXIT_SUCCESS : EXIT_FAILURE);
}

int ypsc_command(const YPSpur_msg* msg, YPSpur_msg* res_msg)
{
  return process_one_command(msg, res_msg);
}

void ypsc_set_odometry_hook(OdometryHook fn)
{
  set_odometry_hook(fn);
}

int ypsc_kill()
{
  if (option(OPTION_WITHOUT_DEVICE))
  {
    g_simulation_exit = 1;
  }
  else
  {
    serial_close();
  }
}

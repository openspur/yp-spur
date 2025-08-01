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

#ifndef YPSPUR_SHVEL_PARAM_H
#define YPSPUR_SHVEL_PARAM_H

#include <assert.h>

typedef union
{
  int integer;
  char byte[4];
} Int_4Char;

typedef union _short_2char
{
  short integer;
  char byte[2];
} Short_2Char;

// typedef
enum
{
  SERVO_LEVEL_STOP = 0,
  SERVO_LEVEL_COUNTER,
  SERVO_LEVEL_TORQUE,
  SERVO_LEVEL_VELOCITY,
  SERVO_LEVEL_POSITION,  // not used
  SERVO_LEVEL_OPENFREE,
};

typedef enum
{
  PARAM_w_ref = 0,
  PARAM_w_ref_highprec,
  PARAM_p_ki,
  PARAM_p_kv,
  PARAM_p_fr_plus,
  PARAM_p_fr_wplus,
  PARAM_p_fr_minus,
  PARAM_p_fr_wminus,
  PARAM_p_A,
  PARAM_p_B,
  PARAM_p_C,
  PARAM_p_D,
  PARAM_p_E,
  PARAM_p_F,
  PARAM_p_pi_kp,
  PARAM_p_pi_ki,
  PARAM_pwm_max,
  PARAM_pwm_min,
  PARAM_toq_max,
  PARAM_toq_min,
  PARAM_int_max,
  PARAM_int_min,
  PARAM_p_toq_offset,
  PARAM_toq_limit,
  PARAM_enc_rev,
  PARAM_motor_phase,
  PARAM_vsrc,
  PARAM_p_inertia_self,
  PARAM_p_inertia_cross,
  PARAM_enc_type,
  PARAM_control_cycle,
  PARAM_enc_div,
  PARAM_enc_denominator,
  PARAM_hall_delay_factor,
  PARAM_lr_cutoff_vel,
  PARAM_vmin,
  PARAM_BLOCK0_END,
  PARAM_servo = 64,
  PARAM_watch_dog_limit,
  PARAM_heartbeat,
  PARAM_BLOCK1_END,
  PARAM_io_dir = 96,
  PARAM_io_data,
  PARAM_ad_mask,
  PARAM_phase_offset,
  PARAM_protocol_version,
  PARAM_ping,
  PARAM_dump,
  PARAM_BLOCK2_END,
} YPSpur_shvel_param;

#ifdef static_assert
static_assert(PARAM_BLOCK0_END <= PARAM_servo, "Parameter enum overwrapped");
static_assert(PARAM_BLOCK1_END <= PARAM_io_dir, "Parameter enum overwrapped");
static_assert(PARAM_BLOCK2_END <= 255, "Parameter enum overflow");
#endif

typedef enum
{
  INT_enc_index_rise = 0,
  INT_enc_index_fall,
  INT_error_state,
  INT_ping_response,
  INT_debug_dump,
} YPSpur_shvel_interrupt;

#define MOTOR_ID_BROADCAST 0x7E

#endif  // YPSPUR_SHVEL_PARAM_H

#ifndef __GLOBALS__
#define __GLOBALS__

#ifdef GLOBAL_DEFINE
#define GLOBAL
#else
#define GLOBAL extern
#endif

#include<sh-serial.h>
#include<motor-device.h>

GLOBAL char cnt_updated;
GLOBAL short watch_dog;
GLOBAL int speed;

GLOBAL short counter[MOTOR_NUM];				// 各チャンネルのカウンタ値
GLOBAL int cnt_dif[MOTOR_NUM];					// WAIT間のカウント数
GLOBAL short counter_buf[MOTOR_NUM];

GLOBAL short analog[8];
GLOBAL unsigned short analog_mask;
GLOBAL unsigned char dio_enable;

/* パラメータ保護用 */
GLOBAL char param_lock;

GLOBAL char param_change;						// 目標値等がかわった際。
GLOBAL char servo_level;						// サーボの状態

/* */
GLOBAL int out_pwm[MOTOR_NUM];
GLOBAL int pwm_sum[MOTOR_NUM];
GLOBAL int pwm_buf[MOTOR_NUM];

GLOBAL int toq[MOTOR_NUM];
GLOBAL int toq_pi[MOTOR_NUM];

/* 目標値 */
GLOBAL int w_ref[MOTOR_NUM * 2];
GLOBAL int w_ref_diff[MOTOR_NUM];
GLOBAL int w_ref_before[MOTOR_NUM];

/* フィードフォワードでのトルク→PWM変換パラメータ */
GLOBAL int p_ki[MOTOR_NUM];
GLOBAL int p_kv[MOTOR_NUM];

/* 摩擦補償 */
GLOBAL int p_fr_plus[MOTOR_NUM];
GLOBAL int p_fr_wplus[MOTOR_NUM];
GLOBAL int p_fr_minus[MOTOR_NUM];
GLOBAL int p_fr_wminus[MOTOR_NUM];

GLOBAL int p_toq_offset[MOTOR_NUM];
GLOBAL int toq_limit[MOTOR_NUM];

/* PWSのパラメータ */
GLOBAL int p_A, p_B, p_C, p_D, p_E, p_F;

/* PI制御のパラメータ */
GLOBAL int p_pi_kp[MOTOR_NUM];
GLOBAL int p_pi_ki[MOTOR_NUM];

/* PWMのリミット */
GLOBAL int pwm_max[MOTOR_NUM];
GLOBAL int pwm_min[MOTOR_NUM];

/* トルクのリミット */
GLOBAL int toq_max[MOTOR_NUM];
GLOBAL int toq_min[MOTOR_NUM];

/* 積分 */
GLOBAL int int_w[MOTOR_NUM];
GLOBAL int int_max[MOTOR_NUM];
GLOBAL int int_min[MOTOR_NUM];

/* ウォッチドックタイマのリミット */
GLOBAL int p_watch_dog_limit;

GLOBAL int s_a, s_b;

/* 通信用 */
GLOBAL char SCI_receive_buffer[2][SCI_RECEIVE_BUFFER_SIZE];	/* リングバッファ */
GLOBAL volatile int SCI_receive_rp[2];							/* リングバッファの読み込みポインタ */
GLOBAL volatile int SCI_receive_wp[2];							/* リングバッファの書き込みポインタ */
GLOBAL char SCI_receive_data[2][100];								/* 受信データのデコード結果の保存 */
GLOBAL volatile int SCI_receive_data_pos[2];						/* 受信データのデコード結果の保存位置 */
GLOBAL int SCI_receive_state[2];
GLOBAL char SCI_send_buffer[2][SCI_RECEIVE_BUFFER_SIZE];		/* リングバッファ */
GLOBAL volatile int SCI_send_rp[2];								/* リングバッファの読み込みポインタ */
GLOBAL volatile int SCI_send_wp[2];								/* リングバッファの書き込みポインタ */

#define EXTENDED_CMD_TEXT_SIZE 100
GLOBAL char extended_cmd_text[EXTENDED_CMD_TEXT_SIZE];	/* 受信データのデコード結果の保存 */
GLOBAL int extended_cmd_text_pos;				/* の書き込み位置・サイズ */

#endif

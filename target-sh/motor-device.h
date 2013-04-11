/* ------------------------------------------------------- */
/* SH2/7045でモータを操作するための関数群　　　　　　　　　　 */
/* 　 　　 ISSのプログラムをベースに作成 */
/* ------------------------------------------------------- */
#ifndef __MOTOR_CONTROL__
#define __MOTOR_CONTROL__

#define MOTOR_NUM 2

#define MTR_MODE_FREE 0
#define MTR_MODE_CW_BRAKE 1
#define MTR_MODE_CCW_BRAKE 2

#define MTR_PWM_DEFAULT_MAX 255

// ///////////////// デューティー比設定 pwm_set //////////////////////
void set_pwm( const unsigned char id, const int duty );
void put_pwm( int ch, int pwm );
void set_mode( const unsigned char id, const int mode );
void put_pwm_free( int ch );

// /////////////////////// init counter ///////////////////////////
void initCounter( void );
// //////////////////////// init PWM //////////////////////////////////
void initPWM(  );
void noPWM_brake(  );
// ///////////////// カウント値取り込み cnt_read ////////////////////
void cnt_read( void );
#endif

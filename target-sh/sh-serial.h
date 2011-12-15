/* --------------------------------------------------- */
/* SH2-PC間のシリアル通信をDTCで行うためのプログラム */
/* ISSのプログラムをベースに作成 */
/* --------------------------------------------------- */
#ifndef __SERIAL__
#define __SERIAL__
#include <sh7040s.h>

// defines
#define SCI_RECEIVE_BUFFER_SIZE 512
#define SCI_SEND_BUFFER_SIZE 512
#define SER_STX 0x09							// start
#define SER_ETX 0x0a							// end

/* 初期化 */
void sci_init( int baud );
void sci_start( void );

/* 送信用関数 */
int encode( unsigned char *src, int len, unsigned char *dst, int buf_max );
void data_send( int channel, short cnt1, short cnt2, short pwm1, short pwm2, short *analog, unsigned short analog_mask );
void sci_send( int channel, char *send_buf, int len );

/* 受信用関数 */
int sci_receive( int channel );

#endif

/*************************************************************/


/**---------------------------------------------------
 * SH2-PC間のシリアル通信をするための関数郡              
 * ISS -> TKU -> OOS -> AWD,STK 
 * 受信はSCI割り込みを使用 
 * ---------------------------------------------------*/
#include <sh7040s.h>
#include <sh-serial.h>
#include <sh-globals.h>
#include <sh-vel.h>
#include <shvel-param.h>

/**---------------------------------------------------
 * SCI INITIALIZATION fixed baud at 38400 
 * ---------------------------------------------------*/
void sci_init( int baud )
{
	// N = ( 28640000 / (32*B) ) - 1 = 895000 / B - 1
	volatile int i, B;

	B = (int)( (float)( 895000.0 / baud ) - 1.0 + 0.5 );
	speed = baud;

	SCI0.SCR.BYTE = 0;							/* clear all flags */
	SCI0.SMR.BYTE = 0;							/* Ascnc, 7bit , NoParity, stop1, 1/1 */
	SCI0.BRR = B;									/* 38400baud (CPU=28.6MHz) */
	SCI1.SCR.BYTE = 0;							/* clear all flags */
	SCI1.SMR.BYTE = 0;							/* Ascnc, 7bit , NoParity, stop1, 1/1 */
	SCI1.BRR = B;									/* 38400baud (CPU=28.6MHz) */

	for ( i = 0; i < 10000; i++ );				/* wait more than 1bit time */

	SCI0.SCR.BYTE = 0x30;						/* scr = 0011 0000 (TE=1,RE=1) */
	SCI1.SCR.BYTE = 0x30;						/* scr = 0011 0000 (TE=1,RE=1) */

	PFC.PACRL2.BIT.PA1MD = 1;					/* 0:PA1, 1:TXD0 */
	PFC.PACRL2.BIT.PA0MD = 1;					/* 0:PA0, 1:RXD0 */
	PFC.PACRL2.BIT.PA4MD = 1;					/* 0:PA4, 1:TXD1 */
	PFC.PACRL2.BIT.PA3MD = 1;					/* 0:PA3, 1:RXD1 */

	return;
}

/* ----------------------送信----------------------------- */
/* 送信 */
void sci_send( int channel, char *send_buf, int len )
{
	volatile struct st_sci *sci;
	int i;

	/* チャンネルの切り替え */
	sci = ( channel == 0 ? &SCI0 : &SCI1 );
	for ( i = 0; i < len; i++ )
	{
		SCI_send_buffer[channel][SCI_send_wp[channel]] = send_buf[i];
		SCI_send_wp[channel] = ( SCI_send_wp[channel] + 1 ) % SCI_SEND_BUFFER_SIZE;
		if( !sci->SCR.BIT.TIE )
		{
			sci->SCR.BIT.TIE = 1;
		}
	}
}

/* 送信データのエンコード */
int encode( unsigned char *src, int len, unsigned char *dst, int buf_max )
{
	int pos, s_pos, w_pos;

	unsigned short b;

	pos = 0;									// read position
	w_pos = 1;									// write_position
	s_pos = 0;
	b = 0;

	while( pos < len || s_pos >= 6 )
	{
		if( s_pos >= 6 )
		{
			dst[w_pos] = ( ( b >> 10 ) & 0x3f ) + 0x40;
			w_pos++;
			if( w_pos >= buf_max )
				return ( -1 );
			b = b << 6;
			s_pos -= 6;
		}
		else
		{
			b |= src[pos] << ( 8 - s_pos );
			s_pos += 8;
			pos++;
			if( pos >= len )
				s_pos += 4;						// 最後
		}
	}

	if( ++w_pos >= buf_max )
		return ( -1 );
	dst[0] = 0x09;								// ECC_STX; //0x09
	dst[w_pos - 1] = 0x0a;						// ECC_ETX;//0x0a
	return w_pos;
}

/* オドメトリデータの送信 */
void data_send( int channel, short cnt1, short cnt2, short pwm1, short pwm2, short *analog, unsigned short analog_mask )
{
	char data[34], send_buf[68];

	int len, i;

	data[0] = ( ( Short_2Char ) cnt1 ).byte[0];
	data[1] = ( ( Short_2Char ) cnt1 ).byte[1];
	data[2] = ( ( Short_2Char ) cnt2 ).byte[0];
	data[3] = ( ( Short_2Char ) cnt2 ).byte[1];
	data[4] = ( ( Short_2Char ) pwm1 ).byte[0];
	data[5] = ( ( Short_2Char ) pwm1 ).byte[1];
	data[6] = ( ( Short_2Char ) pwm2 ).byte[0];
	data[7] = ( ( Short_2Char ) pwm2 ).byte[1];

	len = 8;
	for ( i = 0; analog_mask != 0; analog_mask = analog_mask >> 1, i++ )
	{
		if( analog_mask & 1 )
		{
			data[len] = ( ( Short_2Char ) analog[i] ).byte[0];
			data[len + 1] = ( ( Short_2Char ) analog[i] ).byte[1];
			len += 2;
		}
	}

	// 変換
	len = encode( data, len, send_buf, 68 );

	// 送信
	sci_send( channel, send_buf, len );
}

/* ----------------------受信----------------------------- */
/* 受信の開始 */
void sci_start( void )
{
	volatile unsigned char dummy;
	// メモリのクリア
	SCI_receive_rp[0] = SCI_receive_wp[0] = 0;
	SCI_receive_state[0] = SCI_receive_data_pos[0] = 0;
	SCI_receive_rp[1] = SCI_receive_wp[1] = 0;
	SCI_receive_state[1] = SCI_receive_data_pos[1] = 0;
	SCI_send_rp[0] = SCI_send_wp[0] = 0;
	SCI_send_rp[1] = SCI_send_wp[1] = 0;

	dummy = SCI0.SSR.BYTE;
	SCI0.SSR.BYTE = 0;
	dummy = SCI1.SSR.BYTE;
	SCI1.SSR.BYTE = 0;

	// 割り込み許可
	INTC.IPRF.BIT._SCI0 = 10;
	INTC.IPRF.BIT._SCI1 = 10;
	SCI0.SCR.BIT.RIE = 1;
	SCI1.SCR.BIT.RIE = 1;
	SCI0.SCR.BIT.TIE = 1;
	SCI1.SCR.BIT.TIE = 1;
}
void sci_end( void )
{
	volatile unsigned char dummy;
	// メモリのクリア
	SCI_receive_rp[0] = SCI_receive_wp[0] = 0;
	SCI_receive_state[0] = SCI_receive_data_pos[0] = 0;
	SCI_receive_rp[1] = SCI_receive_wp[1] = 0;
	SCI_receive_state[1] = SCI_receive_data_pos[1] = 0;
	SCI_send_rp[0] = SCI_send_wp[0] = 0;
	SCI_send_rp[1] = SCI_send_wp[1] = 0;

	dummy = SCI0.SSR.BYTE;
	SCI0.SSR.BYTE = 0;
	dummy = SCI1.SSR.BYTE;
	SCI1.SSR.BYTE = 0;

	// 割り込み許可
	INTC.IPRF.BIT._SCI0 = 10;
	INTC.IPRF.BIT._SCI1 = 10;
	SCI0.SCR.BIT.RIE = 0;
	SCI1.SCR.BIT.RIE = 0;
	SCI0.SCR.BIT.TIE = 0;
	SCI1.SCR.BIT.TIE = 0;
}

/*************************************************************/

/*-----------バッファ読み込み・コマンド実行-----------*/
int sci_receive( int channel )
{
	// static int state;/*読み込み状態*/
	int receive_length = 0;

	while( SCI_receive_rp[channel] != SCI_receive_wp[channel] )
	{
		char c;

		c = SCI_receive_buffer[channel][SCI_receive_rp[channel]];

		if( SCI_receive_state[channel] == 0 )
		{
			/* スタートバイト待ちステート */
			if( c == SER_STX )
			{
				SCI_receive_state[channel] = 1;
				SCI_receive_data_pos[channel] = 0;
			}
			if( c == SER_ETX )
			{
				extended_cmd_text[extended_cmd_text_pos] = 0;
				if( extended_command_analyze( channel, extended_cmd_text ) == -1 )
				{
					extended_cmd_text_pos = 0;
					return 0;
				}
				extended_cmd_text_pos = 0;
			}
			else
			{
				extended_cmd_text[extended_cmd_text_pos] = c;
				extended_cmd_text_pos++;
				if( extended_cmd_text_pos >= EXTENDED_CMD_TEXT_SIZE )
					extended_cmd_text_pos = EXTENDED_CMD_TEXT_SIZE - 1;
			}

		}
		else
		{
			/* データのデコード処理ステート */
			if( c == SER_ETX )
			{
				/* デコード終了 */
				receive_length = SCI_receive_data_pos[channel];
				SCI_receive_state[channel] = 0;
			}
			else
			{
				switch ( SCI_receive_state[channel] )
				{
					/* データ待ち */
				case 0:
					break;						// 基本的に

					/* 6/24 */
				case 1:
					/* データのデコード */
					SCI_receive_data[channel][SCI_receive_data_pos[channel]] = ( ( c - 0x40 ) << 2 ) & 0xfc;
					SCI_receive_state[channel] = 2;
					break;

					/* 12/24 */
				case 2:
					SCI_receive_data[channel][SCI_receive_data_pos[channel]] |= ( ( ( c - 0x40 ) >> 4 ) & 0x03 );
					SCI_receive_data_pos[channel]++;
					SCI_receive_data[channel][SCI_receive_data_pos[channel]] = ( ( c - 0x40 ) << 4 ) & 0xf0;
					SCI_receive_state[channel] = 3;
					break;

					/* 18/24 */
				case 3:
					SCI_receive_data[channel][SCI_receive_data_pos[channel]] |= ( ( ( c - 0x40 ) >> 2 ) & 0x0f );
					SCI_receive_data_pos[channel]++;
					SCI_receive_data[channel][SCI_receive_data_pos[channel]] = ( ( c - 0x40 ) << 6 ) & 0xc0;
					SCI_receive_state[channel] = 4;
					break;

					/* 24/24 */
				case 4:
					SCI_receive_data[channel][SCI_receive_data_pos[channel]] |= ( ( c - 0x40 ) & 0x3f );
					SCI_receive_data_pos[channel]++;
					SCI_receive_state[channel] = 1;
					break;
				default:
					SCI_receive_state[channel] = 0;	/* 異常・・・ */
					break;
				}
			}
		}

		/* 次のデータへ */
		SCI_receive_rp[channel] = ( SCI_receive_rp[channel] + 1 ) % SCI_RECEIVE_BUFFER_SIZE;

		/* コマンドを受信していたら終了 */
		if( receive_length > 0 )
			return receive_length;
	}
	return 0;
}

/*-----------バッファに書き込むための割り込み-----------*/
#pragma interrupt
int_rxi0(  )
{
	SCI_receive_buffer[0][SCI_receive_wp[0]] = SCI0.RDR;
	SCI_receive_wp[0] = ( SCI_receive_wp[0] + 1 ) % SCI_RECEIVE_BUFFER_SIZE;
	SCI0.SSR.BIT.RDRF = 0;
}

#pragma interrupt
int_rxi1(  )
{
	SCI_receive_buffer[1][SCI_receive_wp[1]] = SCI1.RDR;
	SCI_receive_wp[1] = ( SCI_receive_wp[1] + 1 ) % SCI_RECEIVE_BUFFER_SIZE;
	SCI1.SSR.BIT.RDRF = 0;
}

#pragma interrupt
int_txi0(  )
{
	if( SCI_send_rp[0] == SCI_send_wp[0] )
	{
		SCI0.SCR.BIT.TIE = 0;
		return;
	}

	SCI0.TDR = SCI_send_buffer[0][SCI_send_rp[0]];
	SCI0.SSR.BIT.TDRE = 0;
	/* 次のデータへ */
	SCI_send_rp[0] = ( SCI_send_rp[0] + 1 ) % SCI_SEND_BUFFER_SIZE;
}

#pragma interrupt
int_txi1(  )
{
	if( SCI_send_rp[1] == SCI_send_wp[1] )
	{
		SCI1.SCR.BIT.TIE = 0;
		return;
	}

	SCI1.TDR = SCI_send_buffer[1][SCI_send_rp[1]];
	SCI1.SSR.BIT.TDRE = 0;
	/* 次のデータへ */
	SCI_send_rp[1] = ( SCI_send_rp[1] + 1 ) % SCI_SEND_BUFFER_SIZE;
}


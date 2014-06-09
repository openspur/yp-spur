/* high level I/O */
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <strings.h>

/* low level I/O */
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* SSM 用 */
#ifdef HAVE_SSM
#include <ssm.h>
#include <ssmtype/spur-odometry.h>
#include <ssmtype/pws-motor.h>
#endif

/* yp-spur用 */
#include <communication.h>
#include <serial.h>

/* ライブラリ用 */
#include <ypspur-coordinator.h>

/**
 * @brief エンコード
 */
int encode( unsigned char *src, int len, unsigned char *dst, int buf_max )
{
	int pos, s_pos, w_pos;
	unsigned short b;
	pos = 0;									// read position
	w_pos = 0;									// write_position
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

	if( w_pos >= buf_max )
		return ( -1 );

	return w_pos;
}

/**
 * @brief デコード
 * @param src[in] デコードする文字列
 * @param len[in] デコードする文字列の長さ
 * @param dst[out] デコード後のデータ
 * @param buf_max[in] デコード後のデータバッファのサイズ
 * @return デコード後のバイト数
 */
int decord( unsigned char *src, int len, unsigned char *dst, int buf_max )
{
	unsigned short dat, b;
	int pos, s_pos, w_pos;
	int rerr;
	pos = 0;									// read position
	w_pos = 0;									// write_position
	s_pos = 0;									// shift position
	rerr = 0;
	dat = 0;
	b = 0;
	while( pos < len )
	{
		if( src[pos] >= 0x40 )
			b = src[pos] - 0x40;
		else
			rerr++;

		dat |= ( b << ( 10 - s_pos ) );
		s_pos += 6;
		if( s_pos >= 8 )
		{
			dst[w_pos] = ( dat >> 8 );
			w_pos++;
			if( w_pos >= buf_max )
				return 0;
			s_pos -= 8;
			dat = dat << 8;
		}
		pos++;
	}

	if( rerr )
		return -rerr;
	return w_pos;
}

int encode_write( char *data, int len )
{
	unsigned char buf[128];
	int encode_len, ret;

	buf[0] = COMMUNICATION_START_BYTE;
	encode_len = encode( ( unsigned char * )data, len, buf + 1, 126 );
	buf[encode_len + 1] = COMMUNICATION_END_BYTE;

	ret = serial_write( ( char * )buf, encode_len + 2 );
	if( ret <= 0 )
	{
		return -1;
	}
	serial_flush_out(  );

	return 0;
}

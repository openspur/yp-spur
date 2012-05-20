
/* high level I/O */
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/* low level I/O */
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* ボディパラメータ */
#include <shvel-param.h>

/* yp-spur用 */
#include <serial.h>
#include <odometry.h>
#include <utility.h>

/* ライブラリ用 */
#include <ypspur.h>

int ad[16];										// ad値保存用バッファ
int ad_num = 0;									// adでいくつデータを出力するように指令したか
int dio_num = 0;								// dioでいくつデータを出力するように指令したか

int process_addata( unsigned char *buf, int len )
{
	int j;
	int ad_num;

	ad_num = 0;
	for ( j = 0; j < len; j += 2 )
	{
		Short_2Char data;
		data.byte[1] = buf[j];
		data.byte[0] = buf[j + 1];
		ad[( ( unsigned short )data.integer ) >> 12] = ( unsigned short )data.integer & 0x0FFF;
		// printf("%d=%d\n",((unsigned short)data.integer) >> 12,(unsigned short)data.integer & 0x0FFF);
		ad_num++;
	}
	return ad_num;
}

/* read ad data func for ssm write */
const int *get_addataptr(  )
{
	return ad;
}

int get_addata( int num )
{
	if( num < 0 )
		return 0;
	if( num > 15 )
		return 0;
	return ad[num];
}

int get_ad_num( void )
{
	return ad_num;
}

int get_dio_num( void )
{
	return dio_num;
}

int admask_receive( char *buf, int len, double receive_time, void *data )
{
	buf[len] = 0;
	strcat( ( char * )data, buf );
	if( strstr( ( char * )data, "00P\n\n" ) )
		return -1;
	if( strstr( ( char * )data, "\n\n" ) )
		return -2;
	return 0;
}

int set_admask( unsigned char mask )
{
	/* Send & Recive Buffer */
	char buf[2048];
	int i;

	ad_num = 0;
	strcpy( buf, "\n\n\n\n" );
	serial_write( buf, strlen( buf ) );
	yp_usleep( 50000 );
	serial_flush_in(  );
	yp_usleep( 50000 );
	strcpy( buf, "ADMASK" );
	for ( i = 0; i < 8; i++ )
	{
		if( mask & 0x80 )
		{
			ad_num++;
			strcat( buf, "1" );
		}
		else
			strcat( buf, "0" );
		mask = mask << 1;
	}
	strcat( buf, "\n" );
	serial_write( buf, strlen( buf ) );

	buf[0] = 0;
	if( serial_recieve( admask_receive, buf ) == -1 )
		return 1;

	return 0;
}

int set_diomask( unsigned char enable )
{
	/* Send & Recive Buffer */
	char buf[2048];
	dio_num = 0;

	strcpy( buf, "\n\n\n\n" );
	serial_write( buf, strlen( buf ) );
	yp_usleep( 50000 );
	serial_flush_in(  );
	yp_usleep( 50000 );
	strcpy( buf, "GETIO" );
	if( enable )
	{
		strcat( buf, "1" );
		dio_num++;
	}
	else
		strcat( buf, "0" );
	strcat( buf, "\n" );
	serial_write( buf, strlen( buf ) );

	buf[0] = 0;
	if( serial_recieve( admask_receive, buf ) == -1 )
		return 1;

	return 0;
}

/* high level I/O */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <strings.h>

/* low level I/O */
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

/* ボディパラメータ */
#include <shvel-param.h>

/* yp-spur用 */
#include <serial.h>
#include <utility.h>
#include <param.h>
#include <ypspur-coordinator.h>
#include <yprintf.h>

#include <errno.h>
double SER_BOUDRATE;

/* serial */
#ifndef _WIN32

// Unix用コード
#	include <sys/select.h>
#	include <sys/termios.h>
int g_device_port;
struct termios g_oldtio;

#else

// Windows用コード
#	include <windows.h>
HANDLE g_hdevices;
DCB g_olddcb;
COMMTIMEOUTS g_oldcto;

#endif

#ifndef _WIN32
speed_t i2baud( int baud )
{
	switch ( baud )
	{
	case 0:
		return B0;
	case 9600:
		return B9600;
	case 19200:
		return B19200;
	case 38400:
		return B38400;
#ifdef B57600
	case 57600:
		return B57600;
#endif
#ifdef B128000
	case 128000:
		return B128000;
#endif
#ifdef B115200
	case 115200:
		return B115200;
#endif
#ifdef B230400
	case 230400:
		return B230400;
#endif
#ifdef B460800
	case 460800:
		return B460800;
#endif
#ifdef B576000
	case 576000:
		return B576000;
#endif
#ifdef B921600
	case 921600:
		return B921600;
#endif
	}
	return -1;
}

int baud2i( speed_t baud )
{
	switch ( baud )
	{
	case B0:
		return 0;
	case B9600:
		return 9600;
	case B19200:
		return 19200;
	case B38400:
		return 38400;
#ifdef B57600
	case B57600:
		return 57600;
#endif
#ifdef B128000
	case B128000:
		return 128000;
#endif
#ifdef B115200
	case B115200:
		return 115200;
#endif
#ifdef B230400
	case B230400:
		return 230400;
#endif
#ifdef B460800
	case B460800:
		return 460800;
#endif
#ifdef B576000
	case B576000:
		return 576000;
#endif
#ifdef B921600
	case B921600:
		return 921600;
#endif
	}
	return -1;
}
#else
DWORD i2baud( int baud )
{
	switch ( baud )
	{
	case 9600:
		return CBR_9600;
	case 19200:
		return CBR_19200;
	case 38400:
		return CBR_38400;
	case 115200:
		return CBR_115200;
	case 128000:
		return CBR_128000;
	}
	return CBR_110;
}
#endif

// ポートが接続可能か調べる
int serial_tryconnect( char *device_name )
{
#ifndef _WIN32
	// Unix用
	g_device_port = open( device_name, O_RDWR );
	if( g_device_port < 0 )
	{
		return 0;
	}
	close( g_device_port );
#else
	// Windows用
	// ファイルハンドラの作成
	g_hdevices = CreateFile( device_name, GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0 );
	if( g_hdevices == INVALID_HANDLE_VALUE )
	{
		return 0;
	}
	CloseHandle( g_hdevices );
#endif
	return 1;
}

int recieve_throw( char *buf, int len, double t, void *data )
{
	return 0;
}

/**
 * @brief ボーレートを変更する
 * @param baud[in] (bit/sec) ボーレート
 */
int serial_change_baudrate( int baud )
{
#ifndef _WIN32
	struct termios newtio;
	int ret, errnum;

	memset( &newtio, 0, sizeof ( newtio ) );	// 新しいポートの設定の構造体をクリアする

	// シリアルのボーレートを設定
	// 8bit 通信
	// スタートビット1
	// エンドビット１
	// パリティー無し
	// 合計 10bit

	newtio.c_cflag = CS8 | CLOCAL | CREAD | CRTSCTS;
	errno = 0;
	ret = cfsetispeed( &newtio, i2baud( baud ) );
	if( ret < 0 )
	{
		errnum = errno;
		yprintf( OUTPUT_LV_ERROR, "Error: Failed to set input baud rate to %d\n", baud );
		yprintf( OUTPUT_LV_VERBOSE, "Error: cfsetispeed: %s(%d)\n", strerror( errnum ), errnum );
		return 0;
	}
	errno = 0;
	ret = cfsetospeed( &newtio, i2baud( baud ) );
	if( ret < 0 )
	{
		errnum = errno;
		yprintf( OUTPUT_LV_ERROR, "Error: Failed to set output baud rate to %d\n", baud );
		yprintf( OUTPUT_LV_VERBOSE, "Error: cfsetospeed: %s(%d)\n", strerror( errnum ), errnum );
		return 0;
	}
	SER_BOUDRATE = ( double )baud / 10.0;		// (bit/sec) -> (Byte/sec) 

	newtio.c_iflag = IGNPAR;					// パリティエラーのデータは無視する
	newtio.c_oflag = 1;							// Raw モードでの出力

	newtio.c_cc[VTIME] = 0;						/* キャラクタ間タイマを使わない */
	newtio.c_cc[VMIN] = 1;						/* 1文字来るまで，読み込みをブロックする */

	if( tcsetattr( g_device_port, TCSAFLUSH, &newtio ) )
	{
		errnum = errno;
		yprintf( OUTPUT_LV_ERROR, "Error: Failed to set attribute of serial port\n" );
		yprintf( OUTPUT_LV_VERBOSE, "Error: tcsetattr: %s(%d)\n", strerror( errnum ), errnum );
		return 0;
	}

	{											// ---> check bit rate
		struct termios term;
		speed_t isp, osp;
		errno = 0;

		ret = tcgetattr( g_device_port, &term );
		if( ret < 0 )
		{
			errnum = errno;
			yprintf( OUTPUT_LV_ERROR, "Error: Failed to get  attribute of serial port\n" );
			yprintf( OUTPUT_LV_VERBOSE, "Error: tcgetattr: %s(%d)\n", strerror( errnum ), errnum );
			return 0;
		}
		isp = cfgetispeed( &term );
		osp = cfgetospeed( &term );

		if( baud2i( isp ) != baud || baud2i( osp ) != baud )
		{
			// fail to set bit rate
			yprintf( OUTPUT_LV_ERROR, "Error: Requested baudrate is %d/%d, \n   but sellected baudrate is %d/%d.\n",
					 baud, baud, baud2i( isp ), baud2i( osp ) );
			return 0;
		}
	}											// <--- check bit rate

	ret = write( g_device_port, "\n\nVV\n\n", 6 );
	yp_usleep( 10000 );
	serial_recieve( recieve_throw, NULL );
#else
	// Windows用
	DCB dcb;

	// シリアルポートの状態操作
	GetCommState( g_hdevices, &dcb );			// シリアルポートの状態を取得
	dcb.fBinary = 1;
	dcb.fParity = 0;
	dcb.fOutxCtsFlow = 0;
	dcb.fOutxDsrFlow = 0;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fDsrSensitivity = FALSE;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.ByteSize = 8;
	dcb.BaudRate = i2baud( baud );
	SER_BOUDRATE = ( double )baud;
	SetCommState( g_hdevices, &dcb );			// シリアルポートの状態を設定
#endif
	return 1;
}

// ポートをオープンして 通信の準備をする
int serial_connect( char *device_name )
{
#ifndef _WIN32
	g_device_port = open( device_name, O_RDWR );

	if( g_device_port < 0 )
	{
		yprintf( OUTPUT_LV_ERROR, "Error: Can't open serial port.\n" );
		return 0;
	}

	tcgetattr( g_device_port, &g_oldtio );

	serial_change_baudrate( DEFAULT_BAUDRATE );
#else
	// Windows用
	// DCB dcb;
	COMMTIMEOUTS cto;

	// ファイルハンドラの作成
	g_hdevices = CreateFile( device_name, GENERIC_WRITE | GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
	if( g_hdevices == INVALID_HANDLE_VALUE )
	{
		return 0;
	}
	// シリアルポートの状態操作
	GetCommState( g_hdevices, &g_olddcb );		// シリアルポートの状態を取得
	serial_change_baudrate( DEFAULT_BAUDRATE );

	// シリアルポートのタイムアウト状態操作
	GetCommTimeouts( g_hdevices, &cto );		// タイムアウトの設定状態を取得
	GetCommTimeouts( g_hdevices, &g_oldcto );	// タイムアウトの設定状態を取得
	cto.ReadIntervalTimeout = 0;
	cto.ReadTotalTimeoutMultiplier = 0;
	cto.ReadTotalTimeoutConstant = 0;
	cto.WriteTotalTimeoutMultiplier = 0;
	cto.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts( g_hdevices, &cto );		// タイムアウトの状態を設定
#endif
	return 1;
}

// ポートを閉じる
int serial_close( void )
{
#ifndef _WIN32
	// Unix用
	// 設定を元に戻す
	tcsetattr( g_device_port, TCSANOW, &g_oldtio );
	close( g_device_port );
#else
	// Windows用
	SetCommState( g_hdevices, &g_olddcb );		// シリアルポートの状態を設定
	SetCommTimeouts( g_hdevices, &g_oldcto );	// タイムアウトの状態を設定
	CloseHandle( g_hdevices );
#endif

	return 1;
}

void serial_flush_in( void )
{
#ifndef _WIN32
	// Unix用
	tcflush( g_device_port, TCIFLUSH );
#else
	// Windows用
	char buf[4000];

	DWORD len;
	DWORD ret;
	COMSTAT state;

	while( 1 )
	{
		ClearCommError( g_hdevices, &ret, &state );
		len = state.cbInQue;

		if( len > 0 )
		{
			if( len > 4000 )
				len = 4000;
			if( !ReadFile( g_hdevices, buf, len, &len, NULL ) )
			{
				return;
			}
			break;
		}
		else if( len == 0 )
			break;
		yp_usleep( 5000 );
	}
#endif
}

void serial_flush_out( void )
{
#ifndef _WIN32
	// Unix用
	tcflush( g_device_port, TCOFLUSH );
#else
	// Windows用
#endif
}

// シリアルポートからの受信処理
int serial_recieve( int ( *serial_event ) ( char *, int, double, void * ), void *data )
{
	char buf[4000];
	double receive_time;
	int retval;

	while( 1 )
	{
#ifndef _WIN32
		// Unix用
		fd_set rfds;
		struct timeval tv;
		size_t len;

		tv.tv_sec = 0;
		tv.tv_usec = 200000;
		FD_ZERO( &rfds );
		FD_SET( g_device_port, &rfds );

		retval = select( g_device_port + 1, &rfds, NULL, NULL, &tv );
		if( retval < 0 )
		{
			int errnum;
			errnum = errno;
			yprintf( OUTPUT_LV_VERBOSE, "Error: Select in serial_recieve failed. (%s)\n", strerror( errnum ) );
			return -1;
		}
		else if( retval == 0 )
		{
			yprintf( OUTPUT_LV_VERBOSE, "Error: Select timed out\n" );
			return -1;
		}

		len = read( g_device_port, buf, 4000 );
		if( len < 0 )
		{
			int errnum;
			errnum = errno;
			yprintf( OUTPUT_LV_VERBOSE, "Error: Read in serial_recieve failed. (%s)\n", strerror( errnum ) );
			return -1;
		}
		else if( len == 0 )
		{
			yprintf( OUTPUT_LV_VERBOSE, "Error: Read timed out\n" );
			return -1;
		}
#else
		// Windows用
		DWORD len;
		DWORD ret;
		COMSTAT state;
		/* 
		 * if( WaitForSingleObject( g_hdevices, 1000 ) != WAIT_OBJECT_0 ){ return -1; } */
		while( 1 )
		{
			ClearCommError( g_hdevices, &ret, &state );
			len = state.cbInQue;
			if( len > 4000 )
				len = 4000;
			if( len > 0 )
				break;
			yp_usleep( 5000 );
		}
		if( !ReadFile( g_hdevices, buf, len, &len, NULL ) )
		{
			return -1;
		}
		buf[len] = 0;
#endif
		receive_time = get_time(  );
		buf[len] = 0;

		if( len > 0 )
		{
			retval = serial_event( buf, len, receive_time, data );
			if( retval < 0 )
				return retval;
		}
	}
}

int serial_write( char *buf, int len )
{
#ifndef _WIN32
	// Unix用
	int ret;
#else
	// Windows用
	DWORD ret;
#endif

	do
	{
#ifndef _WIN32
		// Unix用
		ret = write( g_device_port, buf, len );
#else
		// Windows用
		buf[len] = 0;
		WriteFile( g_hdevices, buf, len, ( LPDWORD ) & ret, 0 );
#endif
		// printf("SEND: %s - %d(%d)\n",buf,len,ret);fflush(stdout);
		if( ret <= 0 && len > 0 )
		{
			return -1;
		}
		len -= ret;
		buf += ret;
	}
	while( len > 0 );

#ifdef __APPLE__
	yp_usleep( 100 );
#endif
	return 1;
}

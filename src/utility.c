/* high level I/O */
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <strings.h>
#include <pthread.h>

/* low level I/O */
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <utility.h>

#ifdef __WIN32
#	include <windows.h>
#endif

/* get time stamp */
double get_time( void )
{
	struct timeval current;

	gettimeofday( &current, NULL );

	return current.tv_sec + current.tv_usec / 1000000.0;
}

void yp_usleep( int usec )
{
#if defined(HAVE_NANOSLEEP)
	// nanosleepが利用可能
	struct timespec request;
	request.tv_nsec = usec * 1000;
	request.tv_sec = 0;

	nanosleep( &request, 0 );
#elif defined(__WIN32)
	// MinGWのusleepには1ms以下切り捨ての問題があるためWindows環境ではWinAPIのSleepを使う
	// 1ms以下は切り上げ
	Sleep( ( usec + 999 ) / 1000 );
#else
	// 古いシステム用
	usleep( usec );
#endif
}


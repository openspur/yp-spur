// TODO:あとでコーディングを直す
// 

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <errno.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <communication.h>
#include <ypprotocol.h>
#include <serial.h>
#include <utility.h>
#include <yprintf.h>


int ss_receive( char *buf, int len, double receive_time, void *data )
{
	buf[len] = 0;
	strcat( ( char * )data, buf );
	if( strstr( ( char * )data, "\n\n" ) )
	{
		return -2;
	}
	return 0;
}

int set_baudrate( int baud )
{
	/* Send & Recive Buffer */
	char buf[2048];
	/* Temporary */


	strcpy( buf, "\n\n\n\n" );
	serial_write( buf, strlen( buf ) );
	yp_usleep( 50000 );
	serial_flush_in(  );
	yp_usleep( 50000 );
	sprintf( buf, "SS%06d\n", baud );
	serial_write( buf, strlen( buf ) );

	buf[0] = 0;
	serial_recieve( ss_receive, buf );

	if( strstr( buf, "00P" ) != NULL )
	{
		yp_usleep( 100000 );
		return serial_change_baudrate( baud );
	}
	
	return 0;
}

int vv_receive( char *buf, int len, double receive_time, void *data )
{
	buf[len] = 0;
	strcat( ( char * )data, buf );
	if( strstr( ( char * )data, "\n\n" ) )
	{
		return -2;
	}
	return 0;
}

/**
  @brief Get version info
  @param *apVer Pointer to version structure
  @return failed: 0, succeeded: 1
 */
int get_version( Ver_t * apVer )
{
	/* Send & Recive Buffer */
	char buf[2048], *readpos;
	/* Temporary */
	char *tmp, *lf;
	char *tag, *wbuf;

	readpos = buf;
	memset( apVer, 0, sizeof ( Ver_t ) );

	strcpy( buf, "\n\n\n\n" );
	serial_write( buf, strlen( buf ) );
	yp_usleep( 50000 );
	serial_flush_in(  );
	yp_usleep( 50000 );
	strcpy( buf, "VV\n" );
	serial_write( buf, strlen( buf ) );

	buf[0] = 0;
	serial_recieve( vv_receive, buf );

	while( 1 )
	{
		if( ( lf = strchr( readpos, '\n' ) ) == NULL )
			break;
		*lf = 0;
		if( ( tag = strstr( readpos, "VEND:" ) ) != 0 )
		{
			wbuf = ( apVer->vender );
		}
		else if( ( tag = strstr( readpos, "PROD:" ) ) != 0 )
		{
			wbuf = ( apVer->product );
		}
		else if( ( tag = strstr( readpos, "FIRM:" ) ) != 0 )
		{
			wbuf = ( apVer->firmware );
		}
		else if( ( tag = strstr( readpos, "PROT:" ) ) != 0 )
		{
			wbuf = ( apVer->protocol );
		}
		else if( ( tag = strstr( readpos, "SERI:" ) ) != 0 )
		{
			wbuf = ( apVer->serialno );
		}
		else
		{
			readpos = lf + 1;
			continue;
		}
		tmp = strchr( tag, ';' );
		if( tmp )
		{
			*tmp = 0;
			if( wbuf )
				strcpy( wbuf, tag + 5 );
		}
		readpos = lf + 1;
	}
	return 1;
}

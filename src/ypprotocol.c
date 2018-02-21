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

	if( strstr( buf, "\n00P\n" ) != NULL )
	{
		yp_usleep( 100000 );
		return serial_change_baudrate( baud );
	}
	// RS-232cインターフェース未搭載
	else if( strstr( buf, "\n04T\n" ) != NULL ) {
		SER_BAUDRATE = 0;
		yp_usleep( 100000 );
		return 4;
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
	char *tmp, *lf, *val;
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
	if( strstr( buf, "\n00P\n" ) == 0 )
	{
		return 0;
	}

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
		val = strchr( tag, ':' );
		if( tmp && val )
		{
			*tmp = 0;
			if( wbuf )
			{
				strcpy( wbuf, val + 1 );
			}
		}
		readpos = lf + 1;
	}
	return 1;
}

/**
  @brief Get version info
  @param *apVer Pointer to version structure
  @return failed: 0, succeeded: 1
 */
int get_parameter( Param_t * apParam )
{
	/* Send & Recive Buffer */
	char buf[2048], *readpos;
	/* Temporary */
	char *tmp, *lf, *val;
	char *tag, *wbuf;

	readpos = buf;
	memset( apParam, 0, sizeof ( Param_t ) );

	strcpy( buf, "\n\n\n\n" );
	serial_write( buf, strlen( buf ) );
	yp_usleep( 50000 );
	serial_flush_in(  );
	yp_usleep( 50000 );
	strcpy( buf, "PP\n" );
	serial_write( buf, strlen( buf ) );

	buf[0] = 0;
	serial_recieve( vv_receive, buf );
	if( strstr( buf, "\n00P\n" ) == 0 )
	{
		return 0;
	}

	while( 1 )
	{
		if( ( lf = strchr( readpos, '\n' ) ) == NULL )
			break;
		*lf = 0;
		if( ( tag = strstr( readpos, "PWMRES:" ) ) != 0 )
		{
			wbuf = ( apParam->pwm_resolution );
		}
		else if( ( tag = strstr( readpos, "MOTORNUM:" ) ) != 0 )
		{
			wbuf = ( apParam->motor_num );
		}
		else if( ( tag = strstr( readpos, "NAME:" ) ) != 0 )
		{
			wbuf = ( apParam->robot_name );
		}
		else
		{
			readpos = lf + 1;
			continue;
		}
		tmp = strchr( tag, ';' );
		val = strchr( tag, ':' );
		if( tmp && val )
		{
			*tmp = 0;
			if( wbuf )
			{
				strcpy( wbuf, val + 1 );
			}
		}
		readpos = lf + 1;
	}
	return 1;
}

int get_embedded_param( char *param )
{
	/* Send & Recive Buffer */
	char buf[2048], *readpos;
	/* Temporary */
	char *lf;

	readpos = buf;
	memset( buf, 0, sizeof ( buf ) );

	strcpy( buf, "\n\n\n\n" );
	serial_write( buf, strlen( buf ) );
	yp_usleep( 50000 );
	serial_flush_in(  );
	yp_usleep( 50000 );
	strcpy( buf, "GETEMBEDDEDPARAM\n" );
	serial_write( buf, strlen( buf ) );

	buf[0] = 0;
	serial_recieve( vv_receive, buf );
	if( strstr( buf, "\n00P\n" ) == 0 )
	{
		return 0;
	}

	while( 1 )
	{
		if( ( lf = strchr( readpos, '\n' ) ) == NULL )
			break;
		*lf = 0;
		if( strstr( readpos, "00P" ) != 0 )
		{
			lf++;
			break;
		}
		readpos = lf + 1;
	}
	if( lf == NULL )
		return 0;
	strcpy( param, lf );
	return 1;
}

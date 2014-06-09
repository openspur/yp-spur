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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* ボディパラメータ */
#include <shvel-param.h>

/* yp-spur用 */
#include <serial.h>
#include <adinput.h>
#include <param.h>

/* ライブラリ用 */
#include <ypspur.h>
#include <cartesian2d.h>

void get_ad_com( double *data, double *resdata )
{
	int num;

	num = ( int )data[0];
	resdata[0] = get_addata( num );
}

void set_io_dir_com( double *data, double *resdata )
{
	int num;

	num = ( int )data[0];
	parameter_set( PARAM_io_dir, 0, num );
}

void set_io_data_com( double *data, double *resdata )
{
	int num;

	num = ( int )data[0];
	parameter_set( PARAM_io_data, 0, num );
}

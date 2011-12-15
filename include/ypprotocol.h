// TODO:あとでコーディングを直す
// 

#ifndef __VERSION_H__
#define __VERSION_H__

#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** Structure of VV command */
	typedef struct VERSION_INFO
	{
		char vender[128];
		char product[128];
		char firmware[128];
		char protocol[128];
		char serialno[128];
	} Ver_t;

	int get_version( Ver_t * apVer );
	int set_baudrate( int baud );

#ifdef __cplusplus
}
#endif
#endif

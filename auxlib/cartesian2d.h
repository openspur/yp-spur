#ifndef __COORDINATE_SYSTEM2D__
#define __COORDINATE_SYSTEM2D__

#ifdef __cplusplus
extern "C"
{
#endif

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

	typedef struct cs_t *CSptr;
	typedef struct cs_t
	{
		double x;
		double y;
		double theta;
		CSptr parent;
		CSptr child;
		CSptr brother;
		int level;
	  /**/} CoordinateSystem;

	CSptr CS_add( CSptr parent_cs, double x, double y, double theta );
	int CS_delete( CSptr target_cs );
	int CS_set( CSptr target_cs, double x, double y, double theta );
	int CS_set_on_CS( CSptr target_cs, CSptr on_cs, double x, double y, double theta );
	void CS_turn_base( double *x, double *y, double *theta );

	void CS_recursive_trans( CSptr target_cs, CSptr now_cs, double *x, double *y, double *theta );
	void inv_trans_cs( CSptr target_cs, double *x, double *y, double *theta );
	void trans_cs( CSptr target_cs, double *x, double *y, double *theta );
	void trace_trans_cs( CSptr target_cs, double *x, double *y, double *theta );

#ifdef __cplusplus
}
#endif
#endif

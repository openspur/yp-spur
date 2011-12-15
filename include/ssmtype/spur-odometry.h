#ifndef __SPUR_ODOMETRY_H__
#define __SPUR_ODOMETRY_H__

#define SNAME_ODOMETRY "spur_odometry"
#define SNAME_GLOBAL   "spur_global"
#define SNAME_ADJUST   "spur_adjust"

typedef struct
{
	double x;
	double y;
	double theta;
	double v;
	double w;
} Spur_Odometry;

typedef struct
{
	double radius_r;
	double radius_l;
	double tread;
} Spur_Odometry_Property;

#endif

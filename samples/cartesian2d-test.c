#include <stdio.h>
#include <math.h>
#include <cartesian2d.h>

CSptr BS, GL, LC, LC2, LC3, LC4;

int main( int argc, char *argv[] )
{
	double x, y, theta;

	BS = CS_add( 0, 0, 0, 0 );
	GL = CS_add( BS, 1, 1, 0 );
	LC = CS_add( GL, 1, 1, 0 );
	LC2 = CS_add( GL, 2, 2, 0 );
	LC3 = CS_add( BS, -3, -3, 0 );
	LC4 = CS_add( LC3, -3, -3, 0 );

	x = 2;
	y = 2;
	theta = 2;
	CS_recursive_trans( GL, BS, &x, &y, &theta );
	printf( "%f %f %f\n", x, y, theta );

	x = 2;
	y = 2;
	theta = 2;
	CS_recursive_trans( LC, GL, &x, &y, &theta );
	printf( "%f %f %f\n", x, y, theta );

	x = 2;
	y = 2;
	theta = 2;
	CS_recursive_trans( LC2, GL, &x, &y, &theta );
	printf( "%f %f %f\n", x, y, theta );

	x = 2;
	y = 2;
	theta = 2;
	CS_recursive_trans( LC4, LC, &x, &y, &theta );
	printf( "%f %f %f\n", x, y, theta );

	return 0;
}

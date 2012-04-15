#include <divider.h>

#define define_divider( func, divider ) void func( int *val ){ *val /= divider; }

define_divider( divider_div0, 1 );
define_divider( divider_div1, 1 );
define_divider( divider_div2, 2 );
define_divider( divider_div3, 3 );
define_divider( divider_div4, 4 );
define_divider( divider_div5, 5 );
define_divider( divider_div6, 6 );
define_divider( divider_div7, 7 );
define_divider( divider_div8, 8 );
define_divider( divider_div9, 9 );
define_divider( divider_div10, 10 );
define_divider( divider_div11, 11 );
define_divider( divider_div12, 12 );
define_divider( divider_div13, 13 );
define_divider( divider_div14, 14 );
define_divider( divider_div15, 15 );
define_divider( divider_div16, 16 );
define_divider( divider_div17, 17 );
define_divider( divider_div18, 18 );
define_divider( divider_div19, 19 );
define_divider( divider_div20, 20 );
define_divider( divider_div21, 21 );
define_divider( divider_div22, 22 );
define_divider( divider_div23, 23 );
define_divider( divider_div24, 24 );
define_divider( divider_div25, 25 );
define_divider( divider_div26, 26 );
define_divider( divider_div27, 27 );
define_divider( divider_div28, 28 );
define_divider( divider_div29, 29 );
define_divider( divider_div30, 30 );
define_divider( divider_div31, 31 );
define_divider( divider_div32, 32 );
define_divider( divider_div33, 33 );
define_divider( divider_div34, 34 );
define_divider( divider_div35, 35 );
define_divider( divider_div36, 36 );
define_divider( divider_div37, 37 );
define_divider( divider_div38, 38 );
define_divider( divider_div39, 39 );
define_divider( divider_div40, 40 );
define_divider( divider_div41, 41 );
define_divider( divider_div42, 42 );
define_divider( divider_div43, 43 );
define_divider( divider_div44, 44 );
define_divider( divider_div45, 45 );
define_divider( divider_div46, 46 );
define_divider( divider_div47, 47 );
define_divider( divider_div48, 48 );

void divider_divinf( int *val ){ *val = 0; }

void (*divider[divider_max])( int * ) =
{
	divider_div0,
	divider_div1,
	divider_div2,
	divider_div3,
	divider_div4,
	divider_div5,
	divider_div6,
	divider_div7,
	divider_div8,
	divider_div9,
	divider_div10,
	divider_div11,
	divider_div12,
	divider_div13,
	divider_div14,
	divider_div15,
	divider_div16,
	divider_div17,
	divider_div18,
	divider_div19,
	divider_div20,
	divider_div21,
	divider_div22,
	divider_div23,
	divider_div24,
	divider_div25,
	divider_div26,
	divider_div27,
	divider_div28,
	divider_div29,
	divider_div30,
	divider_div31,
	divider_div32,
	divider_div33,
	divider_div34,
	divider_div35,
	divider_div36,
	divider_div37,
	divider_div38,
	divider_div39,
	divider_div40,
	divider_div41,
	divider_div42,
	divider_div43,
	divider_div44,
	divider_div45,
	divider_div46,
	divider_div47,
	divider_div48,
	divider_divinf,
};


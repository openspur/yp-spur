#include <stdio.h>
#include <unistd.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <ypspur.h>
#include <utility.h>

int main( int argc, char **argv )
{
	Spur_init(  );

	Spur_set_vel( 0.1 );

	Spur_stop(  );
	yp_usleep( 100000 );

	Spur_free(  );

	return 0;
}

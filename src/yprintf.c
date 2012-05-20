/* high level I/O */
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <stdarg.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <param.h>

void yprintf( ParamOutputLv level, const char* format, ... )
{
	va_list ap;

	if( output_lv(  ) < level ) return;

	va_start( ap, format );
	vfprintf( stderr, format, ap );
	va_end( ap );

	fflush( stderr );
}

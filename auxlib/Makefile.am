AUTOMAKE_OPTIONS = foreign 1.10
SUBDIRS = formula-calc

# set the include path found by configure
AM_CPPFLAGS = $(all_includes) -I$(top_srcdir)/auxlib
AM_CFLAGS   = -Wall
AM_CXXFLAGS = $(AM_CFLAGS)

if HAVE_SSM
lib_LTLIBRARIES = libcarte2d.la libodomssm.la
else
lib_LTLIBRARIES = libcarte2d.la
endif

if HAVE_SSM
include_HEADERS = cartesian2d.h odometry-ssm.h
#pkginclude_HEADERS = cartesian2d.h odometry-ssm.h
else
include_HEADERS = cartesian2d.h
#pkginclude_HEADERS = cartesian2d.h
noinst_HEADERS = odometry-ssm.h
endif

## libcarte2d
libcarte2d_la_SOURCES = libcarte2d.c

libcarte2d_la_CFLAGS = $(AM_CFLAGS) -Werror
libcarte2d_la_CXXFLAGS = $(libcarte2d_la_CFLAGS)

#---------------------------------------------------
# ***_la_LDFLAGS = -version-info  CURRENT:REVISION:AGE
# 1. Bugfix                          C   : R++ : A
# 2. Add Interface                   C++ : R=0 : A++
# 3. Delete or Change Interface      C++ : R=0 : A=0

libcarte2d_la_LDFLAGS = -version-info 0:0:0
#libcarte2d_la_LDFLAGS = -release @PACKAGE_VERSION@

## libodomssm
if HAVE_SSM
libodomssm_la_SOURCES = libodomssm.c

libodomssm_la_CFLAGS = $(AM_CFLAGS) -Werror
libodomssm_la_CXXFLAGS = $(libodomssm_la_CFLAGS)

libodomssm_la_LDFLAGS = -lssm -version-info 0:1:0
#libodomssm_la_LDFLAGS = -lssm -release @PACKAGE_VERSION@
endif

AUTOMAKE_OPTIONS = foreign 1.10

# set the include path found by configure
AM_CPPFLAGS = $(all_includes) -I$(top_srcdir)/include -I$(top_srcdir)/auxlib
AM_CFLAGS   = -Wall
AM_CXXFLAGS = $(AM_CFLAGS)

noinst_PROGRAMS = run-test cartesian2d-test MOI-estimate

run_test_SOURCES = run-test.c
run_test_LDFLAGS = ../src/libypspur.la $(NOINST_LT_LDFLAGS) $(LT_LDFLAGS)

MOI_estimate_SOURCES = MOI-estimate.c
MOI_estimate_LDFLAGS = ../src/libypspur.la $(NOINST_LT_LDFLAGS) $(LT_LDFLAGS)

cartesian2d_test_SOURCES = cartesian2d-test.c
cartesian2d_test_LDFLAGS = ../auxlib/libcarte2d.la $(NOINST_LT_LDFLAGS) $(LT_LDFLAGS)


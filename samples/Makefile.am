AUTOMAKE_OPTIONS = foreign 1.10

# set the include path found by configure
INCLUDES= $(all_includes) -I../include -I../auxlib

AM_CFLAGS   = -Wall
AM_CXXFLAGS = $(AM_CFLAGS)

noinst_PROGRAMS = run-test cartesian2d-test MOI-estimate

run_test_SOURCES = run-test.c
run_test_LDFLAGS = -L../src -lypspur -lm

MOI_estimate_SOURCES = MOI-estimate.c
MOI_estimate_LDFLAGS = -L../src -lypspur -lm

cartesian2d_test_SOURCES = cartesian2d-test.c
cartesian2d_test_LDFLAGS = -L../auxlib -lcarte2d -lm

# set the include path found by configure
AM_CPPFLAGS = $(all_includes) -I$(top_srcdir)/src
AM_CFLAGS   = -Wall
AM_CXXFLAGS = $(AM_CFLAGS)

lib_LTLIBRARIES = libformula-calc.la

bin_PROGRAMS = 
noinst_PROGRAMS = formula-test

formula_test_SOURCES = formula-test.c
formula_test_LDFLAGS = $(LT_LDFLAGS)
formula_test_LDADD   = libformula-calc.la

libformula_calc_la_SOURCES = libformula-calc.c
libformula_calc_la_CFLAGS = $(AM_CFLAGS) -Werror
libformula_calc_la_LDFLAGS = $(LT_LDFLAGS) -version-info 0:0:0

include_HEADERS = formula-calc.h
noinst_HEADERS = formula.h


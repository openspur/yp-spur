# set the include path found by configure
INCLUDES= $(all_includes) -I./

AM_CFLAGS   = -Wall
AM_CXXFLAGS = $(AM_CFLAGS)

lib_LTLIBRARIES = libformula-calc.la

bin_PROGRAMS = 
noinst_PROGRAMS = formula-test

formula_test_SOURCES = formula-test.c
formula_test_LDFLAGS = -L./ -lformula-calc -lm
formula_test_DEPENDENCIES = libformula-calc.la

libformula_calc_la_SOURCES = libformula-calc.c
libformula_calc_la_CFLAGS = $(AM_CFLAGS) -Werror
libformula_calc_la_LDFLAGS = -version-info 0:0:0

include_HEADERS = formula-calc.h
noinst_HEADERS = formula.h


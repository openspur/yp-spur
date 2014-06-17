#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([formula-calc], [0.0.1], [atsushi.w at ieee.org])
AM_INIT_AUTOMAKE

AM_MAINTAINER_MODE([disable])

LT_INIT

AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_STRTOD
AC_CHECK_LIB(m, pow)

AC_OUTPUT


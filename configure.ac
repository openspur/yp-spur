#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

#*********************************************
# NOTICE
#---------------------------------------------
# if you change with "libypspur, libodomssm, libcarte2d",
# you MUST change
#    ***_la_LDFLAGS = -version-info C:R:A
# in Makefile.am of src/, auxlib/
#*********************************************

AC_PREREQ(2.61)
AC_INIT([yp-spur], [1.14.0], [platform@roboken.esys.tsukuba.ac.jp])
AM_INIT_AUTOMAKE

AM_MAINTAINER_MODE([disable])

AC_CONFIG_SRCDIR([src/libypspur.c])
AC_CONFIG_HEADERS([include/config.h])
AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_SUBDIRS(auxlib/formula-calc)

# Define Vendor
AC_DEFINE(YP_VENDOR_NAME, "Univ. of Tsukuba - Intelligent Robot Lab.", vendor)
AC_DEFINE(YP_PRODUCT_NAME, "Yamabico Project - Spur", product)
AC_DEFINE(YP_FIRMWARE_NAME, "2013.10.18", firmware)
AC_DEFINE(YP_PROTOCOL_NAME, "YPP:06:00", protocol)

# etc.
AC_DEFINE(YP_PARAMS_DIR, "robot-params", parametor files dir)

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AM_PROG_CC_C_O
AC_PROG_INSTALL

AH_TEMPLATE([HAVE_PKG_CONFIG],[Define if you have pkg-config.])
AC_CHECK_PROGS(PKG_CONFIG, pkg-config,, $PATH)
if test -n "$PKG_CONFIG"; then
	AC_DEFINE(HAVE_PKG_CONFIG)
fi

# Checks for SSM
AC_CHECK_PROGS(SSM_COORDINATOR, ssm-coordinator,, $PATH)
AC_CHECK_HEADERS(ssm.h, HAVE_SSM_H=yes, HAVE_SSM_H=no)
AC_ARG_ENABLE([ssm],
        AS_HELP_STRING([--enable-ssm], [Enable SSM output]),
        [],
	[enable_ssm=yes])
AM_CONDITIONAL(HAVE_SSM, test -n "$SSM_COORDINATOR" -a -n "$HAVE_SSM_H" -a "x$enable_ssm" == xyes )
AM_COND_IF(HAVE_SSM, [AC_MSG_WARN([********** with SSM **********])], [AC_MSG_WARN([********** w/o SSM **********])])
AM_COND_IF(HAVE_SSM, [AC_DEFINE(HAVE_SSM, 1, [Define if you want to compile with SSM])])

# Checks for SH_CROSS
AC_CHECK_PROGS(SH_CROSS_CC, sh-coff-gcc sh-hitachi-coff-gcc,, $PATH)
AC_CHECK_PROGS(SH_CROSS_AR, sh-coff-ar sh-hitachi-coff-ar,, $PATH)
AC_CHECK_PROGS(SH_CROSS_OBJCOPY, sh-coff-objcopy sh-hitachi-coff-objcopy,, $PATH)
AM_CONDITIONAL(HAVE_SH_CROSS, test -n "$SH_CROSS_CC")

AC_CHECK_PROGS(SH_CROSS_ELF_CC, sh-elf-gcc sh-hitachi-elf-gcc,, $PATH)
AC_CHECK_PROGS(SH_CROSS_ELF_AR, sh-elf-ar sh-hitachi-elf-ar,, $PATH)
AC_CHECK_PROGS(SH_CROSS_ELF_OBJCOPY, sh-elf-objcopy sh-hitachi-elf-objcopy,, $PATH)
AM_CONDITIONAL(HAVE_SH_ELF_CROSS, test -n "$SH_CROSS_ELF_CC")


# Checks for libraries.
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h sys/termios.h unistd.h sys/msg.h])

AC_CHECK_LIB( rt, clock_nanosleep )

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_VOLATILE
AC_HEADER_TIME

# Checks for library functions.
AC_FUNC_STRTOD
AC_CHECK_FUNCS([nanosleep])
AC_CHECK_FUNCS([siglongjmp])
AC_CHECK_FUNCS([longjmp])
AC_CHECK_FUNCS([getline])
AC_CHECK_FUNCS([fork])
AC_CHECK_FUNCS([strtok_r])
AC_CHECK_LIB(termcap, tputs)
AC_CHECK_LIB(readline, readline)
AC_CHECK_LIB(m, pow)
AC_CHECK_LIB(pthread, pthread_create,, [AC_MSG_ERROR([libpthread required])])

AC_CHECK_LIB(ws2_32, main)


AC_SYS_POSIX_TERMIOS

AC_ARG_ENABLE([debug],
        AS_HELP_STRING([--enable-debug], [Enable all debugging message]),
        AC_DEFINE(DEBUG,, [Enable all debugging message]))


# output
AC_CONFIG_FILES([
 Makefile
 include/Makefile
 auxlib/Makefile
 src/Makefile
 target-sh/Makefile
 samples/Makefile
])

# utilities/Makefile
AC_ARG_ENABLE([ldconfig],AC_HELP_STRING([--enable-ldconfig],
		[Execute ldconfig after install]))
if test "x$enable_ldconfig" != "xyes"; then
	LDCONFIG="#ldconfig"
else
	AC_PATH_PROG(LDCONFIG, ldconfig, true, [/sbin /usr/sbin $PATH])
fi
AC_SUBST(LDCONFIG)


AC_CANONICAL_HOST
case $host_os in
	cygwin* | mingw*)
		AC_MSG_WARN([***** Configured for Cygwin and MinGW environment *****])
		AC_ENABLE_STATIC
		NOINST_LT_LDFLAGS="-all-static"
		AC_CHECK_PROGS(DLL_PEXPORTS, pexports,, $PATH)
		AC_CHECK_TOOL(DLL_DLLTOOL, dlltool,, $PATH)
		if test "x$enable_shared" = xyes; then
			ENABLE_DLL=yes
		fi
		;;
	*)
		AC_ENABLE_SHARED
		AC_ENABLE_STATIC
		;;
esac
AC_SUBST(NOINST_LT_LDFLAGS)
AC_CHECK_PROGS(DLL_CP, cp,, $PATH)

AC_ARG_ENABLE(all-static, AS_HELP_STRING([--enable-all-static],
              [Build fully static binaries.]),
	[
		if test "$enableval" = "yes" ; then
			LT_LDFLAGS="-all-static $LT_LDFLAGS"
			case $host_os in
				mingw*)
					CFLAGS="-DPTW32_STATIC_LIB $CFLAGS"
					;;
			esac
			AC_MSG_WARN([***** Fully static link enabled *****])
		fi
	])
AC_SUBST(LT_LDFLAGS)

AM_CONDITIONAL(HAVE_PEXPORTS, test -n "$DLL_PEXPORTS")
AM_CONDITIONAL(HAVE_DLLTOOL, test -n "$DLL_DLLTOOL")
AM_CONDITIONAL(BUILD_DLL_FOR_WIN32, test "x$ENABLE_DLL" != "x")

if test "x$BUILD_DLL_FOR_WIN32" != "x"; then
	AM_COND_IF(HAVE_PEXPORTS, [AC_MSG_WARN([********** with pexports **********])], [AC_MSG_WARN([********** w/o pexports **********])])
	AM_COND_IF(HAVE_DLLTOOL, [AC_MSG_WARN([********** with dlltool **********])], [AC_MSG_WARN([********** w/o dlltool **********])])
	if test -z "$DLL_PEXPORTS"; then
		AC_MSG_WARN([***************************************])
		AC_MSG_WARN([* Consider installing pexports        *])
		AC_MSG_WARN([*      to generate .lib for MS VC++   *])
		AC_MSG_WARN([***************************************])
	fi
fi


AC_OUTPUT


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
AC_INIT([yp-spur], [1.13.5], [platform@roboken.esys.tsukuba.ac.jp])
AM_INIT_AUTOMAKE

AC_CONFIG_SRCDIR([src/libypspur.c])
AC_CONFIG_HEADER([include/config.h])
AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_SUBDIRS(auxlib/formula-calc)

# Define Vendor
AC_DEFINE(YP_VENDOR_NAME, "Univ. of Tsukuba - Intelligent Robot Lab.", vendor)
AC_DEFINE(YP_PRODUCT_NAME, "Yamabico Project - Spur", product)
AC_DEFINE(YP_FIRMWARE_NAME, "2013.04.02", firmware)
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
AC_CHECK_HEADERS(ssm.h, AC_DEFINE(HAVE_SSM,, [Define if you have SSM.]))
AC_CHECK_PROGS(SSM_COORDINATOR, ssm-coordinator,, $PATH)
AM_CONDITIONAL(HAVE_SSM, test -n "$SSM_COORDINATOR")

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
AC_FUNC_MALLOC
AC_FUNC_STRTOD
#AC_CHECK_FUNCS([gettimeofday])
AC_CHECK_FUNCS([nanosleep])
AC_CHECK_FUNCS([siglongjmp])
AC_CHECK_FUNCS([longjmp])
AC_CHECK_FUNCS([getline])
AC_CHECK_FUNCS([fork])
AC_CHECK_LIB(readline, readline)

LD_LIBREADLINE=""
if test "x$HAVE_LIBREADLINE" == "x1"; then
	LD_LIBREADLINE="-lreadline"
fi
AC_SUBST(LD_LIBREADLINE)

AM_SYS_POSIX_TERMIOS

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
AC_ARG_WITH(ldconfig,AC_HELP_STRING([--disable-ldconfig],
		[don't execute ldconfig after install]))
if test "$enable_ldconfig" = "no"; then
	LDCONFIG="#ldconfig"
else
	AC_PATH_PROG(LDCONFIG, ldconfig, true, [/sbin /usr/sbin $PATH])
fi
AC_SUBST(LDCONFIG)


LDFLAG_DLL_FOR_WIN32=""
case "$host" in
i*86-*-cygwin* | i*86-*-mingw*) LDFLAG_DLL_FOR_WIN32="-no-undefined";;
esac

AC_SUBST(LDFLAG_DLL_FOR_WIN32)

if test "x$LDFLAG_DLL_FOR_WIN32" != "x"; then
	AC_CHECK_PROGS(DLL_PEXPORTS, pexports,, $PATH)
	AC_CHECK_PROGS(DLL_DLLTOOL, dlltool,, $PATH)
fi

AC_CHECK_PROGS(DLL_CP, cp,, $PATH)

AM_CONDITIONAL(HAVE_PEXPORTS, test -n "$DLL_PEXPORTS")
AM_CONDITIONAL(HAVE_DLLTOOL, test -n "$DLL_DLLTOOL")
AM_CONDITIONAL(BUILD_DLL_FOR_WIN32, test "x$LDFLAG_DLL_FOR_WIN32" != "x")

if test "x$LDFLAG_DLL_FOR_WIN32" != "x"; then
	if test -z "$DLL_PEXPORTS"; then
		AC_MSG_WARN([***************************************])
		AC_MSG_WARN([* Consider installing pexports        *])
		AC_MSG_WARN([*      to generate .lib for MS VC++   *])
		AC_MSG_WARN([***************************************])
	fi
fi


#AC_CONFIG_SUBDIRS([target-sh])

# Check for 64 bit
if test "x$prefix" = "xNONE" ; then
  prefix="/usr/local"
fi

case "$CFLAGS" in
  *-m64*)
    libdir="$prefix/lib64"
    ;;
  *-m32*)
    libdir="$prefix/lib"
    ;;
  *)
    ;;
esac

CFLAGS+=" -Wall -O2"
CXXFLAGS=CFLAGS
AC_SUBST(CFLAGS)
AC_SUBST(CXXFLAGS)

AC_OUTPUT


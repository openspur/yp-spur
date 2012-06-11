AUTOMAKE_OPTIONS = foreign 1.10

# set the include path found by configure
INCLUDES= $(all_includes) -I../include -I../auxlib

AM_CFLAGS   = -Wall
AM_CXXFLAGS = $(AM_CFLAGS)

lib_LTLIBRARIES = libypspur.la

bin_PROGRAMS = ypspur-coordinator ypspur-free ypspur-interpreter


## libypspur
libypspur_la_SOURCES = libypspur.c ipcommunication.c utility.c

libypspur_la_CFLAGS = $(AM_CFLAGS) -Werror
libypspur_la_CXXFLAGS = $(libypspur_la_CFLAGS)

#---------------------------------------------------
# ***_la_LDFLAGS = -version-info  CURRENT:REVISION:AGE
# 1. Bugfix                          C   : R++ : A
# 2. Add Interface                   C++ : R=0 : A++
# 3. Delete or Change Interface      C++ : R=0 : A=0

libypspur_la_LDFLAGS = -version-info 15:2:1




if BUILD_DLL_FOR_WIN32
lib_LTLIBRARIES += libypspur_dll.la

libypspur_dll_la_CFLAGS = $(libypspur_la_CFLAGS)
libypspur_dll_la_SOURCES = libypspur.c ipcommunication.c utility.c
libypspur_dll_la_CXXFLAGS = $(libypspur_la_CXXFLAGS)
libypspur_dll_la_LDFLAGS = $(libypspur_la_LDFLAGS)
libypspur_dll_la_LDFLAGS += -module -shared $(LDFLAG_DLL_FOR_WIN32)

#HAVE_PEXPORTS

LIBYPSPUR_DLL = libypspur_dll.dll
# Current version -1
LIBYPSPUR_LIB = libypspur_dll.lib
LIBYPSPUR_DEF = libypspur_dll.def

$(LIBYPSPUR_DLL) : $(lib_LTLIBRARIES)
	$(DLL_CP) ./.libs/libypspur_dll-*.dll $(LIBYPSPUR_DLL)

if HAVE_PEXPORTS
$(LIBYPSPUR_DEF) : $(LIBYPSPUR_DLL)
	$(DLL_PEXPORTS) $(LIBYPSPUR_DLL) > $(LIBYPSPUR_DEF)

if HAVE_DLLTOOL
$(LIBYPSPUR_LIB) : $(LIBYPSPUR_DLL) $(LIBYPSPUR_DEF)
	$(DLL_DLLTOOL) --dllname $(LIBYPSPUR_DLL) --input-def $(LIBYPSPUR_DEF) --output-lib $(LIBYPSPUR_LIB)
endif

all: $(LIBYPSPUR_LIB)

endif

endif

## ypspur-coordinator
ypspur_coordinator_SOURCES = ypspur-coordinator.c control_vehicle.c \
  control_motion.c param.c ipcommunication.c command_ad.c yprintf.c \
  command.c command_param.c command_set.c command_run.c command_get.c serial.c \
  ypprotocol.c odometry.c utility.c ssm_spur_handler.c communication.c adinput.c

#ypspur_coordinator_CFLAGS = $(AM_CFLAGS) -O -DYP_PARAMS_DIR=`pkg-config --variable=YP_PARAMS_DIR yp-robot-params`
ypspur_coordinator_CFLAGS = $(AM_CFLAGS) -O
ypspur_coordinator_CXXFLAGS = $(ypspur_coordinator_CFLAGS)
ypspur_coordinator_DEPENDENCIES = ../auxlib/libcarte2d.la


if HAVE_SSM
ypspur_coordinator_LDADD = libypspur.la ../auxlib/libcarte2d.la -lssm \
                           -lm -lpthread
else
ypspur_coordinator_LDADD = libypspur.la ../auxlib/libcarte2d.la \
                           -lm -lpthread
endif

## ypspur-free
ypspur_free_SOURCES = ypspur-free.c utility.c
ypspur_free_LDADD = libypspur.la
ypspur_free_DEPENDENCIES = libypspur.la

## ypspur-interpreter
bin_PROGRAMS += ypspur-interpreter
ypspur_interpreter_SOURCES = ypspur-interpreter.c utility.c
ypspur_interpreter_LDADD = libypspur.la $(LD_LIBREADLINE)
ypspur_interpreter_DEPENDENCIES = libypspur.la




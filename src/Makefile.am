AUTOMAKE_OPTIONS = foreign 1.10

# set the include path found by configure
AM_CPPFLAGS = $(all_includes) -I$(top_srcdir)/include -I$(top_srcdir)/auxlib -I$(top_srcdir)/auxlib/formula-calc/src
AM_CFLAGS   = -Wall
AM_CXXFLAGS = $(AM_CFLAGS)

lib_LTLIBRARIES = libypspur.la libypspur-md.la

bin_PROGRAMS = ypspur-coordinator ypspur-free ypspur-interpreter

## libypspur-md
libypspur_md_la_SOURCES = libypspur-md.c ipcommunication.c utility.c yprintf.c msq.win32.c
libypspur_md_la_CFLAGS = $(AM_CFLAGS) -Werror
libypspur_md_la_CXXFLAGS = $(libypspur_la_CFLAGS)

#---------------------------------------------------
# ***_la_LDFLAGS = -version-info  CURRENT:REVISION:AGE
# 1. Bugfix                          C   : R++ : A
# 2. Add Interface                   C++ : R=0 : A++
# 3. Delete or Change Interface      C++ : R=0 : A=0
libypspur_md_la_LDFLAGS = -version-info 15:4:1

## libypspur
libypspur_la_SOURCES = libypspur.c $(libypspur_md_la_SOURCES)
libypspur_la_CFLAGS = $(libypspur_md_la_CFLAGS)
libypspur_la_CXXFLAGS = $(libypspur_md_la_CXXFLAGS)
libypspur_la_LDFLAGS = $(libypspur_md_la_LDFLAGS)


if BUILD_DLL_FOR_WIN32
libypspur_md_dll_la_SOURCES = libypspur-md.c ipcommunication.c utility.c yprintf.c msq.win32.c
libypspur_md_dll_la_CFLAGS = $(libypspur_la_CFLAGS)
libypspur_md_dll_la_CXXFLAGS = $(libypspur_la_CXXFLAGS)
libypspur_md_dll_la_LDFLAGS = $(libypspur_la_LDFLAGS) -module -shared -no-undefined -avoid-version

libypspur_dll_la_SOURCES = libypspur.c $(libypspur_md_dll_la_SOURCES)
libypspur_dll_la_CFLAGS = $(libypspur_md_dll_la_CFLAGS)
libypspur_dll_la_CXXFLAGS = $(libypspur_md_dll_la_CXXFLAGS)
libypspur_dll_la_LDFLAGS = $(libypspur_md_dll_la_LDFLAGS)

LIBYPSPUR_MD_DLL = libypspur-md_dll.dll
LIBYPSPUR_MD_LIB = libypspur-md_dll.lib
LIBYPSPUR_MD_DEF = libypspur-md_dll.def
LIBYPSPUR_DLL = libypspur_dll.dll
LIBYPSPUR_LIB = libypspur_dll.lib
LIBYPSPUR_DEF = libypspur_dll.def

lib_LTLIBRARIES += libypspur_dll.la libypspur_md_dll.la

$(LIBYPSPUR_MD_DLL): $(lib_LTLIBRARIES)
	cp .libs/$(subst -,_,$(LIBYPSPUR_MD_DLL)) $(LIBYPSPUR_MD_DLL)
$(LIBYPSPUR_DLL): $(lib_LTLIBRARIES)
	cp .libs/$(subst -,_,$(LIBYPSPUR_DLL)) $(LIBYPSPUR_DLL)

if HAVE_PEXPORTS
$(LIBYPSPUR_MD_DEF) : $(LIBYPSPUR_MD_DLL)
	$(DLL_PEXPORTS) $(LIBYPSPUR_MD_DLL) > $(LIBYPSPUR_MD_DEF)
$(LIBYPSPUR_DEF) : $(LIBYPSPUR_DLL)
	$(DLL_PEXPORTS) $(LIBYPSPUR_DLL) > $(LIBYPSPUR_DEF)

if HAVE_DLLTOOL
libypspur_md_dll_ladir = $(libdir)
dist_libypspur_md_dll_la_DATA = $(LIBYPSPUR_MD_LIB)
libypspur_dll_ladir = $(libdir)
dist_libypspur_dll_la_DATA = $(LIBYPSPUR_LIB)

$(LIBYPSPUR_MD_LIB) : $(LIBYPSPUR_MD_DLL) $(LIBYPSPUR_MD_DEF)
	$(DLL_DLLTOOL) --dllname $(LIBYPSPUR_MD_DLL) --input-def $(LIBYPSPUR_MD_DEF) --output-lib $(LIBYPSPUR_MD_LIB)
$(LIBYPSPUR_LIB) : $(LIBYPSPUR_DLL) $(LIBYPSPUR_DEF)
	$(DLL_DLLTOOL) --dllname $(LIBYPSPUR_DLL) --input-def $(LIBYPSPUR_DEF) --output-lib $(LIBYPSPUR_LIB)
endif

all: $(LIBYPSPUR_LIB) $(LIBYPSPUR_MD_LIB)

endif

endif

## ypspur-coordinator
ypspur_coordinator_SOURCES = ypspur-coordinator.c control_vehicle.c \
  control_motion.c param.c ipcommunication.c msq.win32.c command_ad.c yprintf.c \
  command.c command_param.c command_set.c command_run.c command_get.c serial.c \
  ypprotocol.c odometry.c utility.c ssm_spur_handler.c communication.c adinput.c

ypspur_coordinator_CFLAGS = $(AM_CFLAGS) -I$(top_srcdir)/auxlib/formula-calc/src
ypspur_coordinator_CXXFLAGS = $(ypspur_coordinator_CFLAGS)
ypspur_coordinator_LDFLAGS = $(LT_LDFLAGS)
ypspur_coordinator_LDADD = libypspur.la ../auxlib/libcarte2d.la \
                           ../auxlib/formula-calc/src/libformula-calc.la

if HAVE_SSM
ypspur_coordinator_LDFLAGS += -lssm
endif

## ypspur-free
ypspur_free_SOURCES = ypspur-free.c utility.c
ypspur_free_LDADD = libypspur.la
ypspur_free_LDFLAGS = $(LT_LDFLAGS)

## ypspur-interpreter
bin_PROGRAMS += ypspur-interpreter
ypspur_interpreter_SOURCES = ypspur-interpreter.c utility.c
ypspur_interpreter_LDADD = libypspur.la
ypspur_interpreter_LDFLAGS = $(LT_LDFLAGS)




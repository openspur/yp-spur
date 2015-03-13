AUTOMAKE_OPTIONS = foreign 1.10

EXTRA_DIST = sh-vel.c motor-device.c sh-serial.c filter.c filter.h \
             sh-vel.h sh7040s.h sh-globals.h motor-device.h sh-serial.h \
             sh7045linker.x startup.s adc.c adc.h divider.c divider.h

# set the include path found by configure
AM_CPPFLAGS = $(all_includes) -I$(top_srcdir)/include -I$(top_srcdir)/auxlib -I$(top_srcdir)/target-sh -I$(top_builddir)/include

TARGET = sh-vel

if HAVE_SH_ELF_CROSS
MOT = $(TARGET).mot
OUTPUT = $(TARGET).elf
SHCC = $(SH_CROSS_ELF_CC)
SHOBJCOPY = $(SH_CROSS_ELF_OBJCOPY)
else
if HAVE_SH_CROSS
MOT = $(TARGET).mot
OUTPUT = $(TARGET).coff
SHCC = $(SH_CROSS_CC)
SHOBJCOPY = $(SH_CROSS_OBJCOPY)
endif
endif


OBJS = sh-vel.o motor-device.o sh-serial.o adc.o divider.o filter.o
SHLINK = $(top_srcdir)/target-sh/sh7045linker.x
SHSTART = startup.o

SHOPT = -O2
SHCFLAGS = $(AM_CPPFLAGS) -m2 $(SHOPT)
SHLDFLAGS = -m2 -T $(SHLINK) -nostartfiles -lc -lm
SHOBJ = -O srec


all : $(MOT)

$(MOT) : $(OUTPUT)
	$(SHOBJCOPY) $(SHOBJ) $(OUTPUT) $@

.c.o: *.h $(top_srcdir)/include/shvel-param.h Makafile
	$(SHCC) -c $< $(SHCFLAGS)

.s.o:
	$(SHCC) -c $< $(SHCFLAGS)

$(OUTPUT) : $(OBJS) $(SHLINK) $(SHSTART)
	$(SHCC) -o $@ $(OBJS) $(SHSTART) $(SHLDFLAGS)

clean :
	rm -f $(MOT) $(OUTPUT) $(OBJS) $(SHSTART)


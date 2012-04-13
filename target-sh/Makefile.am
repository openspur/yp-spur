EXTRA_DIST = sh-vel.c motor-device.c sh-serial.c \
             sh-vel.h sh7040s.h sh-globals.h motor-device.h sh-serial.h \
             sh7045linker.x startup.s adc.c adc.h

if HAVE_SH_ELF_CROSS

# set the include path found by configure
INCLUDES= $(all_includes) -I../include -I../auxlib -I../target-sh

TARGET = sh-vel
MOT = $(TARGET).mot
ELF = $(TARGET).elf

SRCS = sh-vel.c motor-device.c sh-serial.c adc.c
SHLINK = sh7045linker.x
SHSTART = startup.s

SHCFLAGS = $(INCLUDES) -m2
SHLDFLAGS = -T $(SHLINK) -nostartfiles $(SHSTART) -lc -lm
SHOBJ = -O srec
#srec

all : $(MOT)

$(MOT) : $(ELF)
	$(SH_CROSS_ELF_OBJCOPY) $(SHOBJ) $(ELF) $@

$(ELF) : $(SRCS)
	$(SH_CROSS_ELF_CC) $(SHCFLAGS) $(SHLDFLAGS) $(SRCS) -o $@

clean :
	rm -f $(MOT) $(ELF)

else
if HAVE_SH_CROSS

# set the include path found by configure
INCLUDES= $(all_includes) -I../include -I../auxlib -I../target-sh

TARGET = sh-vel
MOT = $(TARGET).mot
COFF = $(TARGET).coff

SRCS = sh-vel.c motor-device.c sh-serial.c adc.c
SHLINK = sh7045linker.x
SHSTART = startup.s

SHCFLAGS = $(INCLUDES) -m2
SHLDFLAGS = -T $(SHLINK) -nostartfiles $(SHSTART) -lc -lm
SHOBJ = -O srec

all : $(MOT)

$(MOT) : $(COFF)
	$(SH_CROSS_OBJCOPY) $(SHOBJ) $(COFF) $@

$(COFF) : $(SRCS) $(SHLINK) $(SHSTART)
	$(SH_CROSS_CC) $(SHCFLAGS) $(SHLDFLAGS) $(SRCS) -o $@

clean :
	rm -f $(MOT) $(COFF)

endif
endif


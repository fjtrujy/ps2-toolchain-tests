# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.

IRX_DIR = $(PS2_SDK_FOLDER)/iop/irx

#IRX modules
# IRX modules - modules have to be in IRX_DIR
EE_IRXS = iomanX.irx fileXio.irx freesio2.irx 
EE_IRX_CS = $(EE_IRXS:.irx=_irx.c)
EE_IRX_OBJS = $(EE_IRXS:.irx=_irx.o)

EE_BIN = hello.elf
EE_OBJS = $(EE_IRX_OBJS) main.o
EE_LIBS = -lpatches -lfileXio

all: $(EE_IRX_OBJS) $(EE_OBJS) $(EE_BIN)

$(EE_IRX_OBJS):
	bin2c $(IRX_DIR)/$(@:_irx.o=.irx) $(@:.o=.c) $(@:.o=)
	$(EE_CC) $(EE_CFLAGS) $(EE_INCS) -c $(@:.o=.c) -o $@

#Include preferences
include ../Makefile.pref
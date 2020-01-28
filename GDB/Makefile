# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.

EE_BIN = hello.elf
EE_OBJS = main.o ps2ips_irx.c
EE_LIBS = -lpatches -lps2gdbStub -lps2ip -ldebug

#Configuration for IRX
EE_BIN2C = bin2c
IRX_DIR = $(PS2SDK)/iop/irx

#IRX modules
# IRX modules - modules have to be in IRX_DIR
IRX_FILES += ps2ips.irx
IRX_C_FILES = $(IRX_FILES:.irx=_irx.c)

#Include preferences
include ../Makefile.pref

# Specific file name and output per IRX Module
%.irx:
	$(EE_BIN2C) $(IRX_DIR)/$@ $(@:.irx=_irx.c) $(@:.irx=_irx)

irxs: $(IRX_FILES)

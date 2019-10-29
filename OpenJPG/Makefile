# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.

EE_BIN = hello.elf
EE_OBJS = main.o
EE_LIBS = -L$(PS2SDK)/ports/lib -ldebug -ljpeg -lpatches
EE_INCS += -I$(PS2SDK)/ports/include

EE_INCS += -I$(LIBJPEG)/include
EE_CFLAGS += -DHAVE_LIBJPEG
EE_LIB_DIRS += -L$(LIBJPEG)/lib
EE_LIBS += -ljpeg -lfileXio -ldebug

#Include preferences
include ../Makefile.pref
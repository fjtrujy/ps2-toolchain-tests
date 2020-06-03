# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.

EE_BIN = hello.elf
EE_OBJS = main.o cdvd_irx.o
EE_LIBS = -lpatches


#Include preferences
include ../Makefile.pref

irxs: 
	make -C cdfs
	bin2c cdfs/cdvd.irx cdvd_irx.c cdvd_irx

clean2: clean
	make -C cdfs clean
	rm -f cdvd_irx.c


# Execution
# make clean2 && make irxs && make && make run


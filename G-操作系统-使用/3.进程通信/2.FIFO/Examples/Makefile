ROOT=../../..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

SUFFIX = .out
PROGS =	1_ex_fifo \
		2_1_ex_mainserver \
		2_2_ex_mainclient \
		3_ex_pipeconf

FDPASS =

all:	$(PROGS) $(FDPASS)
%:	%.c $(LIBAPUE)
	$(CC) $(CFLAGS) $@.c -o $@$(SUFFIX) $(LDFLAGS) $(LDLIBS)

clean:
	$(RM) $(RMFLAGS) $(PROGS) $(TEMPFILES) *.o

include $(ROOT)/Make.libisshe.inc

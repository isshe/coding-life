ROOT=../..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

PROGS =	1_ex_signal 2_ex_SIGCHLD 3_ex_procmask_bug \
		4_ex_procmask_fix_bug 5_ex_restart
FDPASS =

all:	$(PROGS) $(FDPASS)
%:	%.c $(LIBAPUE)
	$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(PROGS) $(TEMPFILES) *.o

include $(ROOT)/Make.libisshe.inc

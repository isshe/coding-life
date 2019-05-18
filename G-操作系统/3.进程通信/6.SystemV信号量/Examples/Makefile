ROOT=../../..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

SUFFIX = .out
PROGS =	1_ex_sem_create

FDPASS =
LDLIBS += -lpthread

all:	$(PROGS) $(FDPASS)
%:	%.c $(LIBAPUE)
	$(CC) $(CFLAGS) $@.c -o $@$(SUFFIX) $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(PROGS) $(TEMPFILES) *.o

include $(ROOT)/Make.libisshe.inc

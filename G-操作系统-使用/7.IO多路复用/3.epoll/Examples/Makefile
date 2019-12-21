ROOT=../../..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

SUFFIX = .out

PROGS = 1_ex_tcp_echo_server_epoll_01


FDPASS =

all:	$(PROGS) $(FDPASS)
%:	%.c $(LIBAPUE)
	$(CC) $(CFLAGS) $@.c -o $@$(SUFFIX) $(LDFLAGS) $(LDLIBS)

clean:
	$(RM) $(RMFLAGS) $(PROGS) $(TEMPFILES) *.o

include $(ROOT)/Make.libisshe.inc

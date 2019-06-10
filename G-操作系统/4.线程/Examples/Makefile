ROOT=../..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

SUFFIX = .out
PROGS =	1_1_ex_echo_server_thread \
		1_2_ex_echo_client

FDPASS =
LDLIBS += -lpthread

all:	$(PROGS) $(FDPASS)
%:	%.c $(LIBAPUE)
	$(CC) $(CFLAGS) $@.c -o $@$(SUFFIX) $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(PROGS) $(TEMPFILES) *.o

include $(ROOT)/Make.libisshe.inc

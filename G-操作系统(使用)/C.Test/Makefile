ROOT=..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

SUFFIX = .out

PROGS = test_isshe_crypto test_isshe_error test_isshe_rand


all:	$(PROGS)
%:	%.c $(LIBAPUE)
	$(CC) $(CFLAGS) $@.c -o $@$(SUFFIX) $(LDFLAGS) $(LDLIBS)

clean:
	$(RM) $(RMFLAGS) $(PROGS) $(TEMPFILES) *.o

include $(ROOT)/Make.libisshe.inc
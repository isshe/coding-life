ROOT=..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

SUFFIX = .out

PROGS =	isshe_socks_parser

ISSHE_SOCK_CLIENT_OBJ = isshe_config_parser.o isshe_socks_parser.o

FDPASS =
LDLIBS += -lpthread -levent

all:	$(PROGS) $(FDPASS)
%:	%.c $(LIBAPUE)
	$(CC) $(CFLAGS) $@.c -o $@$(SUFFIX) $(LDFLAGS) $(LDLIBS)

isshe_socks_parser: $(ISSHE_SOCK_CLIENT_OBJ)
	${CC} ${CFLAGS} -o $@$(SUFFIX) $(ISSHE_SOCK_CLIENT_OBJ) \
		${LIBS} ${LIBS_RPC} $(LDFLAGS) $(LDLIBS)

clean:
	$(RM) $(RMFLAGS) $(PROGS) $(TEMPFILES) *.o

include $(ROOT)/Make.libisshe.inc

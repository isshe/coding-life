ROOT=..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

PROGS =	isshe_socks_parser.out isshe_proxy_server.out

ISSHE_COMMON_OBJ = isshe_config_parser.o isshe_socks_common.o

ISSHE_SOCKS_PARSER_OBJ = $(ISSHE_COMMON_OBJ) isshe_socks_parser.o

ISSHE_PROXY_SERVER_OBJ = $(ISSHE_COMMON_OBJ) isshe_proxy_server.o

FDPASS =
LDLIBS += -lpthread -levent
CFLAGS += -g

all:	$(PROGS) $(FDPASS)
%:	%.c $(LIBAPUE)
	$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

isshe_socks_parser.out: $(ISSHE_SOCKS_PARSER_OBJ)
	${CC} ${CFLAGS} -o $@ $(ISSHE_SOCKS_PARSER_OBJ) \
		${LIBS} ${LIBS_RPC} $(LDFLAGS) $(LDLIBS)

isshe_proxy_server.out: $(ISSHE_PROXY_SERVER_OBJ)
	${CC} ${CFLAGS} -o $@ $(ISSHE_PROXY_SERVER_OBJ) \
		${LIBS} ${LIBS_RPC} $(LDFLAGS) $(LDLIBS)

clean:
	$(RM) $(RMFLAGS) $(PROGS) $(TEMPFILES) *.o

include $(ROOT)/Make.libisshe.inc

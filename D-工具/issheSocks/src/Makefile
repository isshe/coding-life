ROOT=..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

PROGS =	isshe_socks_parser.out

ISSHE_SOCKS_PARSER_OBJ = isshe_config_parser.o isshe_socks_parser.o

FDPASS =
LDLIBS += -lpthread -levent

all:	$(PROGS) $(FDPASS)
%:	%.c $(LIBAPUE)
	$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

isshe_socks_parser.out: $(ISSHE_SOCKS_PARSER_OBJ)
	${CC} ${CFLAGS} -o $@ $(ISSHE_SOCKS_PARSER_OBJ) \
		${LIBS} ${LIBS_RPC} $(LDFLAGS) $(LDLIBS)

clean:
	$(RM) $(RMFLAGS) $(PROGS) $(TEMPFILES) *.o

include $(ROOT)/Make.libisshe.inc

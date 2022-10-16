ROOT=../../..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

SQUARE_NAME = 1_1_ex_square
SUFFIX = .out
PROGS =	$(SQUARE_NAME)_client \
	$(SQUARE_NAME)_server \
	1_2_ex_square_server

FDPASS =
LDLIBS += -lpthread

all:	$(PROGS) $(FDPASS)
%:	%.c $(LIBAPUE)
	$(CC) $(CFLAGS) $@.c -o $@$(SUFFIX) $(LDFLAGS) $(LDLIBS)

$(SQUARE_NAME).h $(SQUARE_NAME)_clnt.c $(SQUARE_NAME)_svc.c $(SQUARE_NAME)_xdr.c:	$(SQUARE_NAME).x
	rpcgen -C $(SQUARE_NAME).x

$(SQUARE_NAME)_clnt.o: $(SQUARE_NAME)_clnt.c $(SQUARE_NAME).h

$(SQUARE_NAME)_svc.o: $(SQUARE_NAME)_svc.c $(SQUARE_NAME).h

$(SQUARE_NAME)_client:	$(SQUARE_NAME).h $(SQUARE_NAME)_client.o $(SQUARE_NAME)_clnt.o $(SQUARE_NAME)_xdr.o
	${CC} ${CFLAGS} -o $@$(SUFFIX) $(SQUARE_NAME)_client.o $(SQUARE_NAME)_clnt.o $(SQUARE_NAME)_xdr.o \
		${LIBS} ${LIBS_RPC} $(LDFLAGS) $(LDLIBS)

$(SQUARE_NAME)_server:	$(SQUARE_NAME).h $(SQUARE_NAME)_server.o $(SQUARE_NAME)_svc.o $(SQUARE_NAME)_xdr.o
	${CC} ${CFLAGS} -o $@$(SUFFIX) $(SQUARE_NAME)_server.o $(SQUARE_NAME)_svc.o $(SQUARE_NAME)_xdr.o \
		${LIBS} ${LIBS_RPC} $(LDFLAGS) $(LDLIBS)

1_2_ex_square_server: $(SQUARE_NAME).h 1_2_ex_square_server.o $(SQUARE_NAME)_svc.o $(SQUARE_NAME)_xdr.o
	${CC} ${CFLAGS} -o $@$(SUFFIX) 1_2_ex_square_server.o $(SQUARE_NAME)_svc.o $(SQUARE_NAME)_xdr.o \
		${LIBS} ${LIBS_RPC} $(LDFLAGS) $(LDLIBS)

clean:
	$(RM) $(RMFLAGS) $(PROGS) $(TEMPFILES) *.o *_clnt.c *_svc.c *_xdr.c $(SQUARE_NAME).h

include $(ROOT)/Make.libisshe.inc

ROOT=..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

LIBMISC	= libisshe.a
OBJS   = isshe_error.o isshe_file.o \
			isshe_process.o isshe_signal.o isshe_stdio.o \
			isshe_unistd.o isshe_pthread.o isshe_time.o \
			isshe_ipc.o isshe_rpc.o isshe_socket.o \
			isshe_rio.o isshe_sbuf.o \
			crypto/isshe_aes.o \
			crypto/isshe_aes_cfb128.o \
			crypto/isshe_md5.o \
			rand/isshe_rand.o

all:	$(LIBMISC)

$(LIBMISC):	$(OBJS)
	$(AR) rv $(LIBMISC) $?
	$(RANLIB) $(LIBMISC)

clean:
	rm -f *.o $(LIBMISC) crypto/*.o rand/*.o

include $(ROOT)/Make.libisshe.inc
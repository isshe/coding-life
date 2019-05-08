ROOT=..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

LIBMISC	= libisshe.a
OBJS   = isshe_common.o isshe_error.o isshe_file.o \
			isshe_process.o isshe_signal.o isshe_stdio.o \
			isshe_unistd.o isshe_pthread.o

# ???
all:	$(LIBMISC)

$(LIBMISC):	$(OBJS)
	$(AR) rv $(LIBMISC) $?
	$(RANLIB) $(LIBMISC)

clean:
	rm -f *.o $(LIBMISC)

include $(ROOT)/Make.libisshe.inc
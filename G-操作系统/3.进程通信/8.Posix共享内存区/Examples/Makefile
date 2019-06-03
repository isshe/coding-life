ROOT=../../..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

SUFFIX = .out
PROGS =	1_ex_shm_create \
	2_ex_shm_unlink \
	3_ex_shm_write \
	4_ex_shm_read \
	5_ex_pxshm_test3 \
	6_1_ex_pxshm_server1 \
	6_2_ex_pxshm_client1 \
	7_2_ex_pxshm_server2

FDPASS =
LDLIBS += -lpthread

all:	$(PROGS) $(FDPASS)
%:	%.c $(LIBAPUE)
	$(CC) $(CFLAGS) $@.c -o $@$(SUFFIX) $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(PROGS) $(TEMPFILES) *.o

include $(ROOT)/Make.libisshe.inc

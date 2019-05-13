ROOT=../../..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

SUFFIX = .out
PROGS =	1_1_ex_sem_create \
	1_2_ex_sem_unlink \
	1_3_ex_sem_getvalue \
	1_4_ex_sem_wait \
	1_5_ex_sem_post \
	2_prod_cons_sem \
	3_ex_file_lock_posix_sem \
	4_ex_multi_prod_single_cons \
	5_ex_multi_prod_multi_cons

FDPASS =
LDLIBS += -lpthread

all:	$(PROGS) $(FDPASS)
%:	%.c $(LIBAPUE)
	$(CC) $(CFLAGS) $@.c -o $@$(SUFFIX) $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(PROGS) $(TEMPFILES) *.o

include $(ROOT)/Make.libisshe.inc

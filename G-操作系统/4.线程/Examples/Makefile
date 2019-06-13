ROOT=../..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

SUFFIX = .out
PROGS =	1_1_ex_echo_server_thread \
		1_2_ex_echo_client \
		1_3_ex_sharing_thread \
		1_4_ex_bad_cnt_thread \
		1_5_ex_good_cnt_thread \
		1_6_ex_echo_server_pre_thread \
		1_7_ex_psum_thread

FDPASS =
LDLIBS += -lpthread

all:	$(PROGS) $(FDPASS)
%:	%.c $(LIBAPUE)
	$(CC) $(CFLAGS) $@.c -o $@$(SUFFIX) $(LDFLAGS) $(LDLIBS)

clean:
	$(RM) $(RMFLAGS) $(PROGS) $(TEMPFILES) *.o

include $(ROOT)/Make.libisshe.inc

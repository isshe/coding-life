CLANG = clang

EXECABLE = hello

BPFCODE = hello_kern

KERNAL_SOURCE = /usr/src/linux-source-5.4.0/linux-source-5.4.0

BPFTOOLS = $(KERNAL_SOURCE)/samples/bpf
BPFLOADER = $(BPFTOOLS)/bpf_load.c

# KERNAL_CFLAGS += -I$(KERNAL_SOURCE)/tools/testing/selftests/bpf

KBUILD_CFLAGS += -I$(KERNAL_SOURCE)/samples/bpf
KBUILD_CFLAGS += -I$(KERNAL_SOURCE)/usr/include
KBUILD_CFLAGS += -I$(KERNAL_SOURCE)/tools/lib/bpf/
KBUILD_CFLAGS += -I$(KERNAL_SOURCE)/tools/testing/selftests/bpf/
KBUILD_CFLAGS += -I$(KERNAL_SOURCE)/tools/lib/
KBUILD_CFLAGS += -I$(KERNAL_SOURCE)/tools/include
KBUILD_CFLAGS += -I$(KERNAL_SOURCE)/tools/perf

LIBRARY_PATH += -L$(KERNAL_SOURCE)/tools/lib/bpf

BPFSO = -lbpf

# Setting -DHAVE_ATTR_TEST=0 for the kernel containing below patch:
# 06f84d1989b7 perf tools: Make usage of test_attr__* optional for perf-sys.h
#
# The patch was included in Linus's tree starting v5.5-rc1, but was also included
# in stable kernel branch linux-5.4.y. So it's hard to determine whether a kernel
# is affected based on the kernel version alone:
# - for a v5.4 kernel from Linus's tree, no;
# - for a v5.4 kernel from the stable tree (used by many distros), yes.
#
# So let's look at the actual kernel source code to decide.
#
# See more context at:
# https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/commit/?id=06f84d1989b7e58d56fa2e448664585749d41221
# https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/commit/?id=fce9501aec6bdda45ef3a5e365a5e0de7de7fe2d
CFLAGS += $(shell grep -q "define HAVE_ATTR_TEST 1" $(KERNAL_SOURCE)/tools/perf/perf-sys.h \
                  && echo "-DHAVE_ATTR_TEST=0")

.PHONY: clean $(CLANG) bpfload build

clean:
	rm -f *.o *.so $(EXECABLE)

build: ${BPFCODE.c} ${BPFLOADER}
	$(CLANG) -O2 -target bpf -c $(BPFCODE:=.c) $(KBUILD_CFLAGS) -o ${BPFCODE:=.o}

bpfload: build
	$(CLANG) $(CFLAGS) -o $(EXECABLE) -lelf $(KBUILD_CFLAGS) $(LIBRARY_PATH) $(BPFSO) \
        $(BPFLOADER) hello_user.c

$(EXECABLE): bpfload

.DEFAULT_GOAL := $(EXECABLE)

run: bpfload
	export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(KERNAL_SOURCE)/tools/lib/bpf; ./hello

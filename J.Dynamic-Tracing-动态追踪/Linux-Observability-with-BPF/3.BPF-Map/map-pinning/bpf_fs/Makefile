CLANG = clang

EXECABLE = map_pinning_save.out

KERNAL_SOURCE = /usr/src/linux-source-5.4.0/linux-source-5.4.0

BPFTOOLS = $(KERNAL_SOURCE)/samples/bpf
BPFLOADER = $(BPFTOOLS)/bpf_load.c

CCINCLUDE += -I$(KERNAL_SOURCE)/tools/testing/selftests/bpf
CCINCLUDE += -I$(KERNAL_SOURCE)/samples/bpf
CCINCLUDE += -I$(KERNAL_SOURCE)/tools/lib
CCINCLUDE += -I$(KERNAL_SOURCE)/tools/lib/bpf
CCINCLUDE += -I$(KERNAL_SOURCE)/tools/perf
CCINCLUDE += -I$(KERNAL_SOURCE)/tools/include


LIBRARY_PATH += -L$(KERNAL_SOURCE)/tools/lib/bpf
BPFSO = -lbpf

CFLAGS += $(shell grep -q "define HAVE_ATTR_TEST 1" $(KERNAL_SOURCE)/tools/perf/perf-sys.h \
                  && echo "-DHAVE_ATTR_TEST=0")

.PHONY: clean $(CLANG)  build

clean:
	rm -f *.o *.so $(EXECABLE)


build:
	clang $(CFLAGS) -o $(EXECABLE) -lelf $(CCINCLUDE) $(LIBRARY_PATH) $(BPFSO) \
        $(BPFLOADER) $(EXECABLE:=.c)

.DEFAULT_GOAL := build

run:
	export LD_LIBRARY_PATH=$(KERNAL_SOURCE)/tools/lib/bpf:$$LD_LIBRARY_PATH; ./$(EXECABLE)

EXECABLE = main.out

KERNAL_SOURCE = /usr/src/linux-source-5.4.0/linux-source-5.4.0

all:
	gcc -o $(EXECABLE) main.c -I$(KERNAL_SOURCE)/tools/lib/ -L$(KERNAL_SOURCE)/tools/lib/bpf -lbpf

run:
	export LD_LIBRARY_PATH=$(KERNAL_SOURCE)/tools/lib/bpf:$$LD_LIBRARY_PATH; ./$(EXECABLE)

clean:
	rm -rf $(EXECABLE)

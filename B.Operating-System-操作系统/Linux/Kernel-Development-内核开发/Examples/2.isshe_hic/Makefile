obj-m += isshe_hic.o

CURRENT_PATH:=$(shell pwd)
LINUX_KERNEL_PATH:=/lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) modules
clean:
	$(MAKE) -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) clean

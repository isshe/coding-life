obj-m += pid_tree_viz.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

insmod:
	sudo insmod pid_tree_viz.ko

rmmod:
	sudo rmmod pid_tree_viz.ko

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

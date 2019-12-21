DIRS = 9.SystemV共享内存区
# 1.管道 2.FIFO 5.Posix信号量 6.SystemV信号量 7.共享内存区 8.Posix共享内存区

all:
	for i in $(DIRS); do \
		(cd $$i && echo "making $$i" && $(MAKE)) || exit 1; \
	done

clean:
	for i in $(DIRS); do \
		(cd $$i && echo "making $$i" && $(MAKE) clean) || exit 1; \
	done
DIRS = B.lib 1.进程 2.信号 3.进程通信

all:
	for i in $(DIRS); do \
		(cd $$i && echo "making $$i" && $(MAKE)) || exit 1; \
	done

clean:
	for i in $(DIRS); do \
		(cd $$i && echo "making $$i" && $(MAKE) clean) || exit 1; \
	done
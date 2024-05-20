DIRS = 1.select 2.poll 3.epoll 4.kqueue

all:
	for i in $(DIRS); do \
		(cd $$i && echo "making $$i" && $(MAKE)) || exit 1; \
	done

clean:
	for i in $(DIRS); do \
		(cd $$i && echo "making $$i" && $(MAKE) clean) || exit 1; \
	done
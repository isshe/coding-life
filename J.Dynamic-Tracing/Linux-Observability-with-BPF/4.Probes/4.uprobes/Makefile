NAME = hello-bpf


all:
	gcc -o $(NAME)-c.out $(NAME).c
	go build -o $(NAME)-go.out $(NAME).go

enable_event:
	cat /sys/kernel/debug/tracing/events/enable
	echo 1 > /sys/kernel/debug/tracing/events/enable
	cat /sys/kernel/debug/tracing/events/enable

run: enable_event
	./$(NAME).out
	python3 ./example.py

nmc: all
	nm $(NAME)-c.out | grep main

nmgo: all
	nm $(NAME)-go.out | grep main

dumpc: all
	objdump -t $(NAME)-c.out | grep main

dumpgo: all
	objdump -t $(NAME)-go.out | grep main

clean:
	rm -rf *.out


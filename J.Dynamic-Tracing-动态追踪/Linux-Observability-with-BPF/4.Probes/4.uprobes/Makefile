NAME = hello-bpf


all:
	gcc -o $(NAME)-c.out $(NAME).c
	go build -o $(NAME)-go.out $(NAME).go

run:
	./$(NAME).out

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


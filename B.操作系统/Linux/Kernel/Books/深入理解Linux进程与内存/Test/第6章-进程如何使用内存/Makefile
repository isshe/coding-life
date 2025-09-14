.PHONY: default
default: help

### help: 展示此帮助信息
.PHONY: help
help:
	@echo Usage: make ...
	@echo
	@grep -E '^### [-A-Za-z0-9_]+:' Makefile | sed 's/###/   /'

### test1: 验证线程栈是否受 ulimit stack size 限制
test1:
	@gcc -o pthread_stack_ulimit.out pthread_stack_ulimit.c -lpthread

	@echo 默认 stack size 信息：
	@./pthread_stack_ulimit.out || true

	@echo 修改 stack size 大小为 512 KB，程序执行结果：
	@ulimit -s 512 && ./pthread_stack_ulimit.out | tail -n 5

	@echo 修改 stack size 大小为 1024 KB，程序执行结果：
	@ulimit -s 1024 && ./pthread_stack_ulimit.out | tail -n 5

### test2: 验证进程栈的限制
test2:
	@gcc -o process_stack_ulimit.out process_stack_ulimit.c
	./process_stack_ulimit.out

### test3: 验证进程栈的动态拓展
test3:
	@gcc -o process_stack_expansion.out process_stack_expansion.c
	./process_stack_expansion.out

### test4: 确认栈布局
test4:
	@gcc -o print_stack_info.out print_stack_info.c
	./print_stack_info.out

### test5: 获取缺页中断处理的调用栈
test5:
	bash ./trace.sh

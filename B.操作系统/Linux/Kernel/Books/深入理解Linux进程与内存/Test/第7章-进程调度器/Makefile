.PHONY: default
default: help

### help: 展示此帮助信息
.PHONY: help
help:
	@echo Usage: make ...
	@echo
	@grep -E '^### [-A-Za-z0-9_]+:' Makefile | sed 's/###/   /'

### test1: 测试进程切换的开销
test1:
	@gcc -o test1.out test1.c
	@./test1.out


### test2: 测试线程切换的开销
test2:
	@gcc -o test2.out test2.c
	@./test2.out

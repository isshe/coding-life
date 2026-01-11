.PHONY: default
default: help

### help: 展示此帮助信息
.PHONY: help
help:
	@echo Usage: make ...
	@echo
	@grep -E '^### [-A-Za-z0-9_]+:' Makefile | sed 's/###/   /'

### test1: 进行内存限制流程及OOM Kill
test1:
	@gcc -g -o test1.out test1.c
	bash ./test1.sh

### test2: 验证内存限制最小值最大值
test2:
	bash ./test2.sh


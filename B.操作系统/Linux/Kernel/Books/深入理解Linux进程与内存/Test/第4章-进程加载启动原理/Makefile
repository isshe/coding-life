.PHONY: default
default: help

### help: 展示此帮助信息
.PHONY: help
help:
	@echo Usage: make ...
	@echo
	@grep -E '^### [-A-Za-z0-9_]+:' Makefile | sed 's/###/   /'

### all: c 程序的入口地址与 so 的入口地址对比
all:
	# 编译但不strip符号
	gcc -o test_entry.out test_entry.c

	# 查看程序入口地址
	readelf -h test_entry.out | grep Entry

	# 查看动态链接器入口地址
	readelf -h /lib64/ld-linux-x86-64.so.2 | grep "Entry point"

	# 查看符号表
	objdump -t test_entry.out | grep -E "(start|main)"

	# 反汇编_start函数
	objdump -d test_entry.out | grep -A 20 -B 2 "_start>"

	# ldd 看链接了哪些库
	ldd test_entry.out

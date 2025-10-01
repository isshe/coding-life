.PHONY: default
default: help

### help: 展示此帮助信息
.PHONY: help
help:
	@echo Usage: make ...
	@echo
	@grep -E '^### [-A-Za-z0-9_]+:' Makefile | sed 's/###/   /'

### test1: 寻找 Go 程序的执行入口
test1:
	@go build -o test1.out test1.go
	@readelf --file-header test1.out | grep 'Entry point address'
	$(eval ENTRY_POINT := $(shell readelf --file-header test1.out | grep 'Entry point address' | awk '{print $$4}' | sed 's/^0x//'))
	@nm -n test1.out | grep "$(ENTRY_POINT)"

.PHONY: default
default: help

### help: 展示此帮助信息
.PHONY: help
help:
	@echo Usage: make ...
	@echo
	@grep -E '^### [-A-Za-z0-9_]+:' Makefile | sed 's/###/   /'

### test1: 计算函数调用消耗多少指令
test1:
	@gcc -g -o test1-1.out test1-1.c
	@gcc -g -o test1-2.out test1-2.c
	@INST1=$$(perf stat ./test1-1.out 2>&1 | grep instructions | awk -F ' ' '{print $$1}'); \
	INST2=$$(perf stat ./test1-2.out 2>&1 | grep instructions | awk -F ' ' '{print $$1}'); \
	echo "含函数调用的总指令数：$$INST1"; \
	echo "不含函数调用的总指令数：$$INST2"; \
	echo "$$INST1" "$$INST2" | awk '{gsub(/,/, "", $$1); gsub(/,/, "", $$2); printf "每次函数调用的指令数：%.2f\n", ($$1 - $$2) / 100000000}'

### test2: Go 语言函数调用原理
test2:
	@go tool compile -S -N -l test2.go | sed 's|/[^:)]*/Test/[^:)]*/||g'

### test3: 不同语言开销对比
test3:
	@go build -gcflags="-m -l" -o test3-1.out test3-1.go
	@go build -gcflags="-m -l" -o test3-2.out test3-2.go
	@TM1=$$({ time ./test1-1.out; } 2>&1 | grep real | awk -F ' ' '{print $$2}' | awk -F 's' '{print $$1}' | awk -F '0m' '{print $$2}'); \
	TM2=$$({ time ./test1-2.out; } 2>&1 | grep real | awk -F ' ' '{print $$2}' | awk -F 's' '{print $$1}' | awk -F '0m' '{print $$2}'); \
	echo "C 语言含函数调用的总时间：$$TM1"; \
	echo "C 语言不含函数调用的总时间：$$TM2"; \
	echo "$$TM1" "$$TM2" | awk '{gsub(/,/, "", $$1); gsub(/,/, "", $$2); printf "每个 C 函数调用耗时：%.2f ns\n", ($$1 - $$2) / 100000000 * 1000000000}'
	@TM1=$$({ time ./test3-1.out; } 2>&1 | grep real | awk -F ' ' '{print $$2}' | awk -F 's' '{print $$1}' | awk -F '0m' '{print $$2}'); \
	TM2=$$({ time ./test3-2.out; } 2>&1 | grep real | awk -F ' ' '{print $$2}' | awk -F 's' '{print $$1}' | awk -F '0m' '{print $$2}'); \
	echo "Go 语言含函数调用的总时间：$$TM1"; \
	echo "Go 语言不含函数调用的总时间：$$TM2"; \
	echo "$$TM1" "$$TM2" | awk '{gsub(/,/, "", $$1); gsub(/,/, "", $$2); printf "每个 Go 函数调用耗时：%.2f ns\n", ($$1 - $$2) / 100000000 * 1000000000}'

### test4: 计算 read 系统调用的耗时
test4:
	@gcc -g -o test4.out test4.c
	@gcc -g -o test4-2.out test4-2.c
	@dd if=/dev/zero of=data.out bs=1M count=1 2>/dev/null
	@TM1=$$({ time ./test4.out; } 2>&1 | grep real | awk -F ' ' '{print $$2}' | awk -F 's' '{print $$1}' | awk -F '0m' '{print $$2}'); \
	INST1=$$(perf stat ./test4.out 2>&1 | grep instructions | awk -F ' ' '{print $$1}'); \
	INST2=$$(perf stat ./test4-2.out 2>&1 | grep instructions | awk -F ' ' '{print $$1}'); \
	echo "100 万次 read 系统调用总耗时：$$TM1 s"; \
	echo "$$INST1" "$$INST2" | awk '{gsub(/,/, "", $$1); gsub(/,/, "", $$2); printf "100 万次 read 系统调用总指令数：%.3f\n", $$1 - $$2}'; \
	echo "$$TM1" | awk '{gsub(/,/, "", $$1); printf "每次 read 系统调用耗时：%.2f ns\n", $$1 / 1000000 * 1000000000}'; \
	echo "$$INST1" "$$INST2" | awk '{gsub(/,/, "", $$1); gsub(/,/, "", $$2); printf "每次 read 函数调用所需指令数：%.3f\n", ($$1 - $$2) / 1000000}'

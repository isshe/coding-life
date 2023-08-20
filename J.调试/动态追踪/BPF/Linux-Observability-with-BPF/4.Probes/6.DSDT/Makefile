EXECABLE = main.out

KERNAL_SOURCE = /usr/src/linux-source-5.4.0/linux-source-5.4.0

all: compile

prepare:
	apt update
	apt install systemtap-sdt-dev

compile:
	gcc -o $(EXECABLE) main.c

run:
	@./$(EXECABLE)
	@echo "please run ./main.out"
	python3 ./main.py

# better tool: https://github.com/iovisor/bcc/blob/master/tools/tplist.py
readelf: compile
	readelf -n ./$(EXECABLE)

tplist: compile
	python3 tplist.py -l ./$(EXECABLE)

clean:
	rm -rf $(EXECABLE)

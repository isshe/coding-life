EXECABLE = main.out

KERNAL_SOURCE = /usr/src/linux-source-5.4.0/linux-source-5.4.0

all: compile

compile:
	go build -o $(EXECABLE) main.go

run:
	@./$(EXECABLE)

example:
	python3 ./example.py

# better tool: https://github.com/iovisor/bcc/blob/master/tools/tplist.py
readelf: compile
	readelf -n ./$(EXECABLE)

tplist: compile
	python3 tplist.py -l ./$(EXECABLE)

clean:
	rm -rf $(EXECABLE)


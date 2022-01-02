EXECABLE = main.out

KERNAL_SOURCE = /usr/src/linux-source-5.4.0/linux-source-5.4.0

all: compile

compile:
	go build -o $(EXECABLE) main.go

kill:
	@pid=$$(pgrep -nx $(EXECABLE)); \
	if [ -n "$$pid" ]; then kill -9 $$pid; fi

run: compile kill
	@./$(EXECABLE) &
	sudo python3 example.py $$(pgrep -nx $(EXECABLE)) > out.profile

flamegraph:
	/root/persional/FlameGraph/flamegraph.pl out.profile > out.svg

# better tool: https://github.com/iovisor/bcc/blob/master/tools/tplist.py
readelf: compile
	readelf -n ./$(EXECABLE)

tplist: compile
	python3 tplist.py -l ./$(EXECABLE)

clean:
	rm -rf $(EXECABLE)


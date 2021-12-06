

.PHONY: enable_event
enable_event:
	cat /sys/kernel/debug/tracing/events/enable
	echo 1 > /sys/kernel/debug/tracing/events/enable
	cat /sys/kernel/debug/tracing/events/enable

.PHONY: run
run: enable_event
	python3 ./example.py

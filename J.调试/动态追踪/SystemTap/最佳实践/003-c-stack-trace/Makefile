.PHONY: default
default: help

#* help:		   Show Makefile rules.
.PHONY: help
help:
	@echo Makefile rules:
	@echo
	@grep -E '^#** [-A-Za-z0-9_]+:' Makefile | sed 's/#**/   /'


.PHONY: run-and-trace convert gen-flame-graph
run-and-trace:
	bash ./1-run-and-trace.sh



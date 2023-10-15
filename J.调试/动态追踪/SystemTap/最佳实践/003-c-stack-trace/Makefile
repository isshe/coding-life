.PHONY: default
default: help

#: help:			Show Makefile rules.
.PHONY: help
help:
	@echo Makefile rules:
	@echo
	@grep -E '^#: [-A-Za-z0-9_]+' Makefile | sed 's/#:/   /'


.PHONY: run-and-trace convert gen-flame-graph

#: run-and-trace:		Start a process and track it
run-and-trace:
	bash ./1-run-and-trace.sh

#: convert			Replace address with name
convert:
	bash ./2-convert.sh

#: gen-flame-graph		Generate falme graph
gen-flame-graph:
	bash ./3-gen-flame-graph.sh

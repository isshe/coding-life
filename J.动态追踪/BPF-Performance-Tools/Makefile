# BCC:
# * CONFIG_BPF=y
# * CONFIG_BPF_SYSCALL=y
# * CONFIG_BPF_EVENTS=y
# * CONFIG_BPF_JIT=y
# * CONFIG_HAVE_EBPF_JIT=y


.PHONY: prepare-ubuntu prepare
prepare-ubuntu:
	sudo apt install bpfcc-tools linux-headers-$(uname -r)

prepare: prepare-ubuntu

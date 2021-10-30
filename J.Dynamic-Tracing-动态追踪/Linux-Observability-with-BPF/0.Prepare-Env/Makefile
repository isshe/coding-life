

prepare-ubuntu:
	apt update
	apt install -y build-essential git make libelf-dev clang llvm strace tar bpfcc-tools gcc-multilib flex bison libssl-dev
	apt install -y linux-source-5.4.0
	cd /usr/src/linux-source-5.4.0; tar xf linux-source-5.4.0.tar.bz2
	cd /usr/src/linux-source-5.4.0/linux-source-5.4.0; cp -v /boot/config-$(uname -r) .config; make headers_install; make M=samples/bpf

prepare: prepare-ubuntu

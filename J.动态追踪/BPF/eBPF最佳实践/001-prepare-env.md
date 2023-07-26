# 环境搭建

# rpm

```
sudo yum install clang llvm elfutils-libelf-devel zlib-devel
```

# deb

**以下为 Ubuntu 20.04 的步骤，仅供参考。**

```bash
apt update
apt install -y build-essential git make libelf-dev clang llvm strace tar bpfcc-tools gcc-multilib flex bison libssl-dev

apt install -y linux-source-`uname -r`
cd /usr/src/linux-source-`uname -r`;
tar xf linux-source-`uname -r`.tar.bz2

cd /usr/src/linux-source-`uname -r`/linux-source`uname -r`
cp -v /boot/config-`uname -r` .config
make headers_install
make M=samples/bpf
```

更多参考：https://www.ebpf.top/post/ubuntu_2104_bpf_env/


# 安装 SystemTap

> 安装 systemtap 的步骤

## 从源码编译

> rockylinux 9 为例。rockylinux 9 安装的 systemtap 是 4.8 版本的，堆栈打印似乎不全。

```bash
git clone https://sourceware.org/git/systemtap.git
cd systemtap
git checkout release-5.0

yum-builddep systemtap

./configure \
    --prefix=/opt/stap \
    --disable-docs \
    --disable-publican \
    --disable-refdocs

make
make install
```

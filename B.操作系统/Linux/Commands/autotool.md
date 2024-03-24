
[TOC]

autotool
---


# 步骤
* 初始化：
  * 进入根目录 autoscan 生成`configure.scan`文件
  * 重命名：`mv configure.scan configure.ac`
* 修改`configure.ac`文件
* `autoreconf --install`生成 configure 等一系列文件
* `./configure`: 生成 makefile
* `make`进行编译

# 常用命令
* `make`：编译源代码，生成目标文件
* `make clean`：清理之前 make 产生的临时文件
* `make distclean`：不仅将 make 产生的文件，同时将 configure 生成的文件也都删除，包括 Makefile
* `make install`：将编译好的可执行文件安装到系统目录，一般为/usr/local/bin
* `make dist`：生成软件发布包，将可执行文件及相关文件打包成”PACKAGE-VERSION.tar.gz”的 tarball。其中 PACKAGE 和 VERSION 可以在 configure.in 中通过 AM_INIT_AUTOMAKE(PACKAGE, VERSION) 定义。
* `make distcheck`：查看发布包是否正确，解压开执行 configure 和 make 来确认

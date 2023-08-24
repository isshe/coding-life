SystemTap
---

# 疑问

1. stap 和 staprun 命令有何区别？

stap 命令执行 SystemTap 脚本时需要编译和加载内核模块，需要管理员权限，而 staprun 命令在用户态下解释执行脚本，无需编译为内核模块，普通用户也可以执行。

2. stap 命令如何只编译不执行？

使用 `-p` 选项，例如 `-p 4`。

# 参考
- https://wiki.ubuntu.com/Kernel/Systemtap
- https://www.cnblogs.com/shuqin/p/13196585.html
- https://github.com/5kg/systemtap/blob/master/testsuite/systemtap.context/backtrace.stp
- https://sourceware.org/systemtap/SystemTap_Beginners_Guide/

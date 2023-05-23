> 待整理

Valgrind 对程序做了什么？

- 程序执行在 Valgrind 核心提供的模拟 CPU 上
- Valgrind 的工具（指定）将自己的检测代码加到程序中，并将结果返回给 Valgrind 核心，核心协调该检测代码继续执行
- Valgrind 模拟程序会执行每一条指令。因此，它不仅会检测指定应用的代码，还会检查所有动态链接库


C++ 程序可以考虑使用 `-fno-inline` 来编译程序，然后再使用 valgrind 检测，才能显示正确的调用链。
此外，也可以使用 Valgrind 的 `-read-inline-info=yes` 选项来执行程序，这样也可以显示正确的调用链。

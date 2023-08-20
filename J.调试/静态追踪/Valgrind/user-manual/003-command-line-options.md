# 命令行选项

## 核心命令行选项

### 工具选择选项

- `--tool=<toolname>` [default: memcheck]

### 基础选项

- `-q, --quiet`: 静默模式，只打印错误信息。
- `trace-children=<yes|no> [default: no]`: 是否跟踪通过 exec 初始化的子程序。
- `--log-*`: 输出的信息发送到哪里去。支持文件描述符(fd)，文件(filename)，套接字(socket)。

### 错误相关选项

- `--demangle=<yes|no> [default: yes]`: 是否自动 demangleing(decoding) C++ 名称。
- `--error-limit=<yes|no> [default: yes]`: 启用后超过 10000000 个错误或者超过 1000 个不同错误后不再报告错误。
- `--extra-debuginfo-path=<path> [default: undefined and unseud]`: 默认情况下 Valgrind 搜索一些总所周知的目录，如 /usr/lib/debug/。可以通过此选项指定额外的 debuginfo 路径。当 Valgrind 搜索 `/w/x/y/zz.so` 并且设置选项 `--extra-debuginfo-path=/a/b/c` 时，实际会搜索 `/a/b/c/w/x/y/zz.so`。
- `supressions=<filename> [default: $PREFIX/lib/valgrind/default.supp]`: 指定抑制文件。
- `--gen-suppressions=<yes|no|all> [default: no]`：生成抑制信息。`yes` 表示一个一个确认再生成；`all` 表示不用确认直接生成。
- `max-threads=<number> [default: 500]`: 默认支持 500 个线程。

### 内存分配相关选项

- `--alignment=<number> [default: 8 or 16, depending on the platform]`: 对齐。

### 非公共选项

### 调试选项

### 设置默认选项（Setting Default Options）

Valgrind 从以下 3 个地方读取选项：

- 文件 `~/.valgrindrc`
  - 示例：`--memcheck:leak-check=yes`，如果不是 Memcheck 工具，则会忽略这个选项。
- 环境变量 `$VALGRIND_OPTS`
- 文件 `./.valgrindrc`

### 动态变更选项（Dynamically Changing Options）


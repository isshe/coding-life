# Valgrind 快速上手

## 1、介绍

Valgrind 工具集提供了一系列调试和剖析工具，可以帮助我们写出更快更正确的程序。
其中最常用的工具是 Memcheck。可以检测很多 C 或 C++ 程序中导致奔溃或未知行为的内存错误。

## 2、准备程序

使用 `-g` 选项编译程序，使程序包含调试信息（debug info），这样可以让 Memcheck 的错误信息包含详细的行号。
如果可以允许程序变慢，使用 `-O0` 来编译最好；如果是 `-O1` 则行号可能有错，不过一般能正常工作，速度相对 `-O0` 也提升明显；`-O2` 或以上的优化级别则不建议使用，可能导致 Memcheck 报告`未初始化值`的错误。

## 使用 Valgrind 执行程序

如果程序正常是使用以下方式执行：

```
myprog arg1 arg
```

则，改为下面的方式：

```bash
valgrind --leak-check=yes myprog arg1 arg2
```

这个命令中没有指定 Memcheck，是因为 Memcheck 是默认的工具；`--leak-check` 选项打开详细的内存泄露检测器。
此时程序可能比平时慢很多（例如 20、30 倍），并且会使用多很多内存。Memcheck 将显示检测到的内存错误或内存泄露信息。

## 解释 Memcheck 的输出

```c
// example.c
#include <stdlib.h>
void f(void)
{
    int* x = malloc(10 * sizeof(int));
    x[10] = 0;  // problem 1: heap block overrun
                // problem 2: memory leak -- x not freed
}

int main(void)
{
   f();
   return 0;
}
```

这是一个有内存泄露和内存错误的程序。

编译：

```bash
gcc ./example.c -g
```

执行：

```bash
valgrind --leak-check=yes ./a.out
```

### 内存访问越界

部分输入如下：

```valgrind
==1765727== Invalid write of size 4
==1765727==    at 0x10916B: f (example.c:5)
==1765727==    by 0x109180: main (example.c:11)
==1765727==  Address 0x4a4e068 is 0 bytes after a block of size 40 alloc'd
==1765727==    at 0x483B7F3: malloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==1765727==    by 0x10915E: f (example.c:4)
==1765727==    by 0x109180: main (example.c:11)
```

这部分输出描述了第一个问题：访问越界。索引应该是 0 - 9，实际访问了 10。
输出中：

- `1765727`：进程 ID。
- `Invalid write of size 4`：第一行说明错误类型。
- `at/by 0xxxxxx`：调用栈。如果调用栈很深，显示不完全，可以通过 `--num-caller` 选项来指定。
- `0x10916B`：这些地址通常来说不重要，但是有时候对于追踪一些奇怪的错误特别有用。
- ` Address 0x4a4e068 is 0 bytes after a block of size 40 alloc'd`：一些错误消息有第二个组成部分，它描述了涉及的内存地址。这表明写入的内存刚好超过 example.c 第 5 行用 malloc() 分配的块的末尾。

通常应该按照报告的顺序来修复错误，因为后面的错误可能是前面的错误引起的。

### 内存泄露

```
==1765727== 40 bytes in 1 blocks are definitely lost in loss record 1 of 1
==1765727==    at 0x483B7F3: malloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==1765727==    by 0x10915E: f (a.c:4)
==1765727==    by 0x109180: main (a.c:11)
```

调用栈说明了泄漏内存的分配位置。
内存泄露有几种不同的类型，其中最重要的两个是：

- `definitely lost`：绝对发生了内存泄露，需要进行修复。
- `probably lost`：可能发生了内存泄漏。除非：正在做一些不常见的事情，例如把指向内存的指针指向已分配内存的中间部分。如果不想看到这些报告，可以使用 `--show-possibly-lost=no` 来关闭。

其他的一些类型：

- `indirectly lost`：间接泄露，意味着正在泄露给予指针结构中的内存。例如：如果二叉树的根节点 `definitely lost`，则所有子节点都将 `indirectly lost`。
- `still reachable`：仍然可访问，意味着程序可能没有问题，只是没有释放一些它可能拥有的内存。这种情况很常见，通常也是合理的。可以使用 `--show-reachable=yes/no` 选项控制是否展示。
- `suppressed`：表示内存泄露消息已经被抑制，可以在 valgrind.suppress 文件中进行配置。可以忽略这些错误。

Memcheck 也会报告使用未初始化的值（uninitialised values）的错误，最常见的是 `Conditional jump or move depends on uninitialised value(s)` 。要确定这些错误的根本原因可能很困难，但可以尝试使用 `--track-origins=yes` 来获得额外的信息。这会导致 Memcheck 运行更慢，但得到额外信息往往可以节省大量的时间来弄清楚未初始化的值来自哪里。

如果不清楚 Memcheck 展示的信息的含义，可以查看 Valgrind 用户手册中的 [Explanation of error messages from Memcheck](https://valgrind.org/docs/manual/mc-manual.html#mc-manual.errormsgs)。

## 附加说明

Memcheck 不是完美的，它可能产生误报，可以通过 valgrind.supress 来控制。详见 Valgrind 用户手册中的 [Suppressing error](https://valgrind.org/docs/manual/manual-core.html#manual-core.suppress)。
不过，通常 99% 都是正确的，所以应该谨慎忽略它的错误消息。
Memcheck 不能检测出程序中所有的内存错误，例如，它无法检测到`静态分配`或`栈上分配`的数组的越界读/写。
尽量让程序达到 Memcheck 不会报告任何错误的状态，这可以更容易知道程序的修改何时会导致 Memcheck 报告新的错误。

## 参考

- [Valgrind FAQ](https://valgrind.org/docs/manual/faq.html#faq.deflost)
- [Valgrind User Manual](https://valgrind.org/docs/manual/manual-core.html)

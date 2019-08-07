[TOC]

# gdb
gdb - GNU调试器。

调试器(如GDB)的目的是允许你查看其他程序在执行时的"内部"的内容，或者程序奔溃的时候正在做什么。
GDB可以做4种主要的事情来帮助你捕捉bug：
* 启动你的程序，指定任何可能影响程序行为的内容。
* 使程序在指定条件下停止。
* 检查你程序停止时，正在发生的事情。
* 更改程序中的内容，以便你可以尝试纠正一个错误的影响并继续了解另一个错误。

你可以用GDB来调试C、C++、Fortran、Modula-2编写的程序。
使用`gdb`命令来调用GDB。一旦启动，它会从中断读取命令，直到你用GDB命令"exit"告诉它退出。你可以用GDB命令`help`来查看帮助（不用退出gdb）。
你可以运行没有参数或选项的gdb ; 但是最常用的启动GDB的方法是使用一个或两个参数，将可执行程序指定为参数：
> gdb program

您还可以从可执行程序和指定的core文件开始：
> gdb program core

如果要调试正在运行的进程，则可以将进程ID指定为第二个参数：
> gdb program 1234
> gdb -p 1234

将GDB附加(attach)到1234进程（除非你由一个名字为1234的core文件，gdb会先查找core文件）。
以下是一些常用的GDB命令:
```bash
break [file:]function
    在函数(文件)中设置断点。
    缩写：b。
    用法：
        b <行号>，如：b 8
        b <函数名称>，如：b main
        b *<函数名称>，如：b *main（在函数名称前面加“*”符号表示将断点设置在“由编译器生成的prolog代码处”）
        b *<代码地址>，如：b *0x804835c。
d [breakpoint number]
    删除断点

run [arglist]
    开始你的程序（arglist是参数列表）
    缩写：r

bt
    回溯(backtrace)：显示程序堆栈。

print expr
    显示表达式expr的值。
    缩写：p

c
    继续运行你的程序（在停止后，例如断电）。continue的缩写。

next
    继续下一行(在停止后)；不进入函数的单步调试。
    缩写：n

edit [file:]function
    查看程序当前停止的行。

list [file:]function
    在当前停止的位置附近键入程序的文本。

step
    继续下一行(在停止后)；进入函数的单步调试。
    缩写：s

i
    显示各类信息。如：i r，显示寄存器的信息

help [name]
    显示GDB命令name的信息，或有关使用GDB的一般信息。

quit
    退出GDB。
    缩写：q
```
有关GDB的完整详细信息，请参阅：A Guide to the GNU Source-Level Debugger

## 用法
```bash
gdb [-help] [-nh] [-nx] [-q] [-batch] [-cd=dir] [-f] [-b bps]
   [-tty=dev] [-s symfile] [-e prog] [-se prog] [-c core] [-p procID]
   [-x cmds] [-d dir] [prog|prog procID|prog core]
```

## 选项
选项之外的任何参数都需要指定可执行文件和核心文件（或进程ID）。
所有选项和命令行参数都按顺序处理。使用' -x '选项时，顺序会有所不同。

```bash
-help
-h  列出所有选项，并提供简要说明。

-symbols=file
-s file
    从file文件中读取符号表（symbol table）。

-write
    允许写入可执行文件和core文件。

-exec=file
-e file
    使用file文件作为可执行文件在适当时执行，并与核心转储一起检查纯数据。

-se=file
    从file文件中读取符号表并将file用作可执行文件。

-core=file
-c file
   指定core文件。

-command=file
-x file
    执行file文件中的GDB命令。（自动化？）

-ex command
    执行给定的GDB命令。

-directory=directory
-d directory
    将目录添加到路径(path)以搜索源文件。

-nh 不要执行~/.gdbinit中的命令。
-nx
-n  不要执行任何'.gdbinit'初始化文件中的命令。

-quiet
-q  "Quiet". 不要打印介绍性和版权信息。这些消息也在批处理模式下被抑制。

-batch
    以批处理模式运行。
    在执行完用-x选项指定的所有命令文件(如果不禁止，则为.gdbinit)后，以0退出。
    如果在运行命令文件中的GDB命令时发生错误，则以非0退出。
    批处理模式可用于将GDB作为过滤器运行，例如在另一台计算机上下载并运行程序; 
    为了使这个更有用，消息程序正常退出。【？？？】

-cd=directory
    使用directory作用GDB的工作目录（而不是用当前目录）

-fullname
-f   Emacs在将GDB作为子进程运行时设置此选项。
    它告诉GDB每次显示堆栈帧时都以标准的，可识别的方式输出完整的文件名和行号（包括每次程序停止时）。
    这种可识别的格式看起来像两个'\032'字符，后跟文件名，行号和以冒号分隔的字符位置，以及换行符。
    Emacs-to-GDB接口程序使用两个'\032'字符作为信号来显示帧的源代码。

-b bps
    设置GDB用于远程调试的任何串行接口的线性速度（波特率或每秒位数）。

-tty=device
    使用device运行程序的标准输入和输出。
```

## 示例
### 编译并跟踪
```bash
gcc test.c -o test -g
gdb test
    <gdb命令> 
    <b main>: 断点
    <r>: 执行
    <c>: 继续执行
    <i r>: 显示寄存器的信息
```

## 疑问

## 拓展
* shell命令：info gdb
* [gdb在线文档](https://sourceware.org/gdb/current/onlinedocs/gdb/)
    * [A Guide to the GNU Source-Level Debugger](http://mermaja.act.uji.es/docencia/is37/data/gdb.pdf)
## 参考
* https://man.linuxde.net/gdb
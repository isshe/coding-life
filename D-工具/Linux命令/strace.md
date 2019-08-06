
[TOC]
# strace
strace - 跟踪系统调用和信号。

在最简单的情况下，strace运行指定的命令直到它退出。
它拦截并记录由进程调用的系统调用和进程接收的信号。
每个系统调用的名称，其参数和返回值都打印在`标准错误`或使用`-o选项指定的文件`上。
strace是一种有用的诊断，指导和调试工具。
跟踪中的每一行都包含系统调用`名称`，后跟括号中的`参数`及其`返回值`。如：
```c
open("/dev/null", O_RDONLY) = 3
```
系统调用错误，如：(返回 -1，并打印出错原因)
```c
open("/foo/bar", O_RDONLY) = -1 ENOENT (No such file or directory)
```
信号，如：
```c
sigsuspend([] <unfinished ...>
--- SIGINT (Interrupt) ---
+++ killed by SIGINT +++
```
正在执行系统调用的同时，另一个线程/进程调用另一个系统调用，则strace将尝试保留这些事件的顺序并将正在进行的调用标记为未完成。 
当调用返回时，它将被标记为已恢复：
```c
[pid 28772] select(4, [3], NULL, NULL, NULL <unfinished ...>
[pid 28779] clock_gettime(CLOCK_REALTIME, {1130322148, 939977000}) = 0
[pid 28772] <... select resumed> )      = 1 (in [3])
```

系统调用被中断后重启，如：
```c
read(0, 0x7ffff72cf5cf, 1)              = ? ERESTARTSYS (To be restarted)
--- SIGALRM (Alarm clock) @ 0 (0) ---
rt_sigreturn(0xe)                       = 0
read(0, ""..., 1)                       = 0
```

解引用结构指针，并根据需要显示成员。 在所有情况下，参数都以尽可能类似C的方式格式化。 例如“ls -l /dev/null”：
```c
lstat("/dev/null", {st_mode=S_IFCHR|0666, st_rdev=makedev(1, 3), ...}) = 0
```

解引用字符指针：
```c
read(3, "root::0:0:System Administrator:/"..., 1024) = 422
```

## 用法
```bash
strace  [-CdffhiqrtttTvVxxy]  [-In]  [-bexecve]  [-eexpr]...   [-acolumn] [-ofile] [-sstrsize] [-Ppath]... -ppid... / [-D]
[-Evar[=val]]... [-uusername] command [args]

strace -c[df] [-In] [-bexecve] [-eexpr]...  [-Ooverhead] [-Ssortby] -ppid... / [-D] [-Evar[=val]]... [-uusername]  command
[args]

```
## 选项
```bash
-c          统计系统调用的次数、出错次数。在Linux上，这会尝试显示系统时间（在内核中运行的CPU时间），与墙上时间无关。
            如果 -c 和 -f 或 -F 一起使用，仅保留所有跟踪进程的聚合总计。

-C          与-c类似，但在进程运行时也会打印常规输出。

-D          将跟踪器进程作为分离的孙子(detached grandchild)进程运行，而不是作为tracee的父进程。 
            这通过将tracee保持为调用进程的直接子进程来减少strace的可见效果

-d          在标准错误上输出strace的debug信息。

-f          跟踪由fork/vfork/clone调用所产生的子进程。
            注意，如果是多线程，则"-p PID -f"将附加到PID进程的所有线程，而不仅是附加到带有 "thread_id = PID" 的线程。


-ff         如果"-o filename"选项生效，则每个进程的跟踪信息都将写入filename.pid中，其中pid是每个进程的进程ID。 
            这与-c不兼容，因为不保留每进程计数。

-F          此选项现已过时，它具有与-f相同的功能。

-h          打印帮助。

-i          在系统调用时打印指令指针。

-q          不显示有关附加/分离(attaching/detaching)的消息。
            当输出重定向到文件并且命令是直接运行的(而不是附加(attaching))的，会自动发生这种情况。

-qq         不显示有关进程退出状态的消息。

-r          在进入每个系统调用时打印相对时间戳。 这记录了连续系统调用开始之间的时间差。

-t          在每一行跟踪信息前面添加时间（一天中的时间），最小单位是秒。

-tt         在每一行跟踪信息前面添加时间（一天中的时间），最小单位是毫秒。

-ttt        打印的时间将包括微秒，并且前导部分将被打印为自纪元以来的秒数。

-T          显示系统调用花费的时间。 这记录了每个系统调用的开始和结束之间的时间差。

-v          打印environment、stat、termios等调用的未缩写版本。 这些结构在调用中非常常见，因此默认行为显示结构成员的合理子集。 使用此选项可获得所有细节。

-V          打印strace的版本.

-x          以十六进制字符串格式打印所有非ASCII字符串。

-xx         以十六进制字符串格式打印所有字符串。

-y          打印与文件描述符参数关联的路径。

-a column   对齐特定列中的返回值（默认列40）。

-b syscall  如果到达指定的系统调用，则从跟踪的进程中分离。 目前，仅支持execve系统调用。
            这个选项很有用，如果你想要跟踪多线程的进程(需要-f)，但不想跟踪其（可能非常复杂的）孩子。

-e expr     用于修改要跟踪的事件或如何跟踪它们的一个限定表达式。 表达式的格式是：
                     [qualifier=][!]value1[,value2]...
              
            qualifier是trace，abbrev，verbose，raw，signal，read，write之一，value是依赖于限定符的符号或数字。 
            默认限定符是trace。使用感叹号会取消该组值。 例如，"-e open"表示字面意思"-e trace=open"，这意味着仅跟踪“open”的系统调用。 
            相比之下，“-e trace=open”表示跟踪除open之外的每个系统调用。 
            此外，特殊值all和none都有明显的含义。

            请注意，即使在引用的参数中，某些shell也会使用感叹号进行历史记录扩展。 
            注意，有些shell使用"!"来执行历史记录里的命令，如果是这样，您必须使用反斜杠转义感叹号。

-e trace=set
            只跟踪指定的系统调用.例如:-e trace=open,close,rean,write表示只跟踪这四个系统调用.默认为set=all。

-e trace=file
            只跟踪有关文件操作的系统调用。（跟踪所有以一个文件名作为参数的系统调用）
            你可以看做是：-e trace=open,stat,chmod,unlink,...
            这对于查看进程引用的文件很有用。
            此外，使用缩写将确保您不会意外忘记在列表中包含类似lstat的调用。 

-e trace=process
            只跟踪有关进程控制的系统调用。 这对于查看进程的fork，wait和exec步骤非常有用。

-e trace=network
            跟踪所有与网络相关的系统调用.

-e trace=signal
            跟踪所有与信号相关的系统调用.

-e trace=ipc
            跟踪所有与IPC相关的系统调用.

-e trace=desc
            跟踪所有与文件描述符相关的系统调用.

-e trace=memory
            跟踪与内存映射相关的所有系统调用.

-e abbrev=set
            设定strace输出的系统调用的结果集。"-v"选项等于“abbrev=none”，默认为“abbrev=all”.

-e verbose=set
            对指定系统调用集的结构进行解引用。 默认值为 "verbose=all"。

-e raw=set
            将指定的系统调用的参数以十六进制显示。（为指定的系统调用集打印原始的，未解码的参数。）
            如果您不信任解码或者您需要知道参数的实际数值，这将非常有用。

-e signal=set
            指定跟踪的系统信号.默认为all.如"signal=!SIGIO"(或者signal=!io),表示不跟踪SIGIO信号.

-e read=set 以完整的16进制或ASCII形式，输出所有从指定文件描述符集中读取的数据。例如: "-e read=3,5"，读文件描述符3和5上的所有输入活动。
            请注意请注意，这与read(2)系统调用的正常跟踪无关，该调用由选项"-e trace=read"控制。

-e write=set
            以完整的16进制或ASCII形式，输出所有写到指定文件描述符集的数据。

-I interruptible
            当strace可以被信号中断（例如按^C）
            1：没有信号被阻挡; 
            2：解码系统调用时阻塞致命信号（默认）; 
            3：致命信号总是被阻止（默认为'-o FILE PROG'）;
            4：致命信号和SIGTSTP（^Z）始终被阻止（有助于使strace -o FILE PROG不在^Z上停止）。
-o filename
            把跟踪信息输出到filename指定的文件而不是输出到stderr。
            如果指定"-ff"则会使用filename.pid。
            如果参数以"|"或"!"开始，则参数的其余部分会被视作一个命令，并且所有输出都会通过管道给它(此命令)。【？？？】
            这样可以方便地将调试输出传递给程序，而不会影响已执行程序的重定向。【？？？】
            [If the argument begins with `|' or with `!' then the rest of the argument is treated as a command and all output is piped to it.  
            This is convenient for piping the debugging output to a program without affecting the redirections of executed programs.]

-O overhead
            将跟踪系统调用的overhead（开销）设置为overhead微秒。
            这对于覆盖默认启发式方法非常有用，可以猜测在使用-c选项进行计时系统调用时仅花费多少时间。
            可以通过在没有跟踪的情况下对给定程序运行进行计时（使用time(1)）并将累积的系统调用时间与使用-c产生的总数进行比较来测量启发式的准确性。

-p pid
            使用进程ID pid附加到进程并开始跟踪。
            可以通过键盘中断信号（CTRL-C）随时终止跟踪。 
            strace会通过将自己从跟踪过程中分离出来而让它（它们）继续运行。
            多个-p选项可用于附加到许多进程。 支持-p"`pidof PROG`"语法。

-P path     仅跟踪系统调用访问的路径。 多个-P选项可用于指定多个路径。

-s strsize  指定要打印的最大字符串大小（默认值为32）。 请注意，文件名不被视为字符串，并且始终完整打印。

-S sortby   按指定条件对-c选项打印的直方图的输出进行排序。 
            合法值是时间(time)，系统调用(call)，名称(name)，默认为时间。(根据什么来排序的意思)

-u username 使用用户名所在的"用户ID"，“组ID“和”补充组“来运行命令。
            此选项仅在以root身份运行，并且能够正确执行setuid（和/或）setgid二进制文件时有用。 
            除非使用此选项，否则setuid和setgid程序在没有有效权限的情况下执行。

-E var=val  在其环境变量列表中运行带有var=val的命令。

-E var      在将var传递给命令前，从继承的环境变量列表中将var删除。
```

## 示例

### 显示每个系统调用的相对时间
> strace -r ls
> ls表示ls命令

### 显示系统调用时间（一天当中的时间/纪元时间）
> strace -t ls
> strace -tt ls
> strace -ttt ls

### 打印与文件描述符相关联的路径
> strace -y ls

### 跟踪指定进程
> strace -p 123

### 把输出存在文件中
> strace -o ls.log ls

## 相关
* ltrace(1)
* time(1)
* ptrace(2)
* proc(5)

## 参考
* https://man.linuxde.net/strace
* Linux man page

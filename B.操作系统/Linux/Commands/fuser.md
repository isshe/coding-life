[TOC]

fuser
---

fuser - 报告进程使用的文件和套接字。
fuser 使用指定的文件或文件系统显示进程的 PID。在默认显示模式下，每个文件名后跟一个表示访问类型的字母：
* c: 当前目录——指示进程的工作目录。
* e: 指示该文件为进程的可执行文件（即进程由该文件拉起的）。
* f: 指示该文件被进程打开，默认情况下`f`字符不显示。
* F: 指示该文件被进程打开并写入，默认情况下`F`字符不显示。
* r: 指示该目录为进程的根目录。
* m: 指示进程使用该文件矜夸内存映射，抑或该文件为共享库文件，被进程映射进内存。
如果未访问任何指定的文件或发生致命错误，fuser 将返回非零返回值。如果至少找到一个访问权限，则 fuser 返回 0。
要使用 TCP 和 UDP 套接字查找进程，必须使用-n 选项选择相应的名称空间。

# 用法
```bash
fuser [-fuv] [-a|-s] [-4|-6] [-c|-m|-n space] [ -k [-i] [-M] [-w] [-SIGNAL] ] name ...
fuser -l
fuser -V
```

# 选项
```bash
       -a, --all
              显示命令行中指定的所有文件。默认情况下，仅显示至少一个进程访问的文件。

       -c     与-m 选项相同，用于POSIX兼容性。

       -f     静默忽略，用于POSIX兼容性。

       -k, --kill
              杀死访问指定文件的进程。除非使用-signal更改，否则会发送SIGKILL。
              一个fuser进程绝不会杀死自己，但可能杀死其他fuser进程。
              在定影过程中不会导致死亡本身，而是可能会杀死其他定影过程。
              执行fuser的进程在尝试kill之前，其有效用户ID会被设置为其真实用户ID。【？？？】

       -i, --interactive
              杀死进程前需要用户确认。
              如果没有指定 -k 选项，则此选项会被悄悄忽略。

       -l, --list-signals
              列出所有已知signal名字。

       -m NAME, --mount NAME        【？？？】
              NAME指定了一个已挂载的文件系统或块设备的文件。
              所有访问该文件系统上文件的进程都会被列出来。
              如果指定的是一个目录，将自动变为NAME/。

       -M, --ismountpoint
              仅当NAME指定挂载点时才会满足请求。
              这是一个非常宝贵的安全带，如果NAME恰好不是文件系统，它可以防止你杀死机器。

       -w     仅杀死具有写访问权限的进程。 
              如果-k不存在，则会默默忽略此选项。

       -n SPACE, --namespace SPACE
              选择一个不同的命名空间。
              支持的命名空间问：文件(文件名、默认)、udp(本地UDP端口)、tcp(本地TCP端口)。
              对于端口，可以指定端口号或端口名称。
              如果没有歧义，可以使用缩写：name/space（例如80/tcp）。

       -s, --silent
              静默操作。此模式下将忽略 -v 和 -u 选项。
              -a 不能和 -s 一起使用。
              Silent operation.  -u and -v are ignored in this mode.  -a must not be used with -s.

       -SIGNAL
              在kill进程时使用指定的信号而不是SIGKILL。
              信号可以通过名称（例如-HUP）或数字（例如 -1）指定。
              如果未使用-k选项，则会默默忽略此选项。

       -u, --user
              将进程所有者的用户名附加到每个PID。
              如：fuser /home/isshe -u

       -v, --verbose
              详细模式。进程以类似ps的样式显示。字段PID/USER/COMMAND类似于ps。
              ACCESS字段表示进程如何访问文件。如果访问是由内核进行的，则显示内核而不是PID。
              如：fuser /home/isshe -u -v

       -V, --version
              显示版本信息

       -4, --ipv4
              仅搜索IPv4套接字。此选项不能与-6选项一起使用，并且仅对tcp和udp命名空间有效。

       -6, --ipv6
              仅搜索IPv6套接字。此选项不能与-4选项一起使用，并且仅对tcp和udp命名空间有效。

       -      重置所有选项并将信号设置回SIGKILL。
```

# 示例
## 显示进程的用户名
> fuser /home/isshe -u

## 详细模式
> fuser /home/isshe -u -v

## 杀死以任意方式访问`某文件系统或目录`的所有进程
> fuser -km /home
> fuser -k -m /home
> fuser -k -x -u -c /dev/hd1 
> 

## 判断是否有进程正在访问某文件
```bash
if fuser -s /home/isshe;
then:
    # do something
else
    # do something
fi
```

## 显示占用某个端口的所有进程
> fuser telnet/tcp
> fuser 80/tcp
> sudo fuser ssh/tcp -u

# 疑问

# 参考
* Linux man page
* https://man.linuxde.net/fuser
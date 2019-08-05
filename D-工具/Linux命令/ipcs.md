[TOC]
# ipcs
ipcs - 提供有关ipc设施/资源的信息。显示的信息包括消息列表、共享内存和信号量的信息。

## 用法
```bash
ipcs [-asmq] [-tclup]
ipcs [-smq] -i id
ipcs -h
```

## 选项
ipcs提供有关调用进程`具有读访问权限`的ipc设施/资源的信息。
```bash
-i      选项允许指定特定的资源ID。 仅打印有关此ID的信息。

-h      帮助文档
```

资源可以指定如下：
```bash
-m     共享内存段

-q     消息队列

-s     信号量数组

-a     所有【默认选项】
```

输出格式可以指定如下：
```bash
-t     时间

-p     pid

-c     创建者

-l     限制

-u     总览
```

## 示例
### IPC资源分配总览
> ipcs -u

### 显示IPC资源的系统限制
> ipcs -l

### 显示指定ID的IPC资源信息
> ipcs -m -i 65536

### 显示某类IPC资源的信息
> ipcs -m

## 相关
* ipcrm
* ipcmk

## 参考
* Linux man page

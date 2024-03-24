[TOC]

netstat
---

显示当前的连接情况。（UDP、TCP、UNIX、ICMP 等）
显示路由表、网络接口列表、网络统计信息、多播信息、伪装连接等。
# 1. 介绍
## 1.1 用法
```bash
netstat [-vWeenNcCF] [<Af>] -r
netstat {-V|--version|-h|--help}
netstat [-vWnNcaeol] [<Socket> ...]
netstat { [-vWeenNac] -i | [-cWnNe] -M | -s }
```

## 1.2 选项
```shell
-r, --route              显示路由表
-i, --interfaces         显示接口列表
-g, --groups             显示多播组成员
-s, --statistics         显示网络统计信息
-M, --masquerade         display masqueraded connections【？？？】

-v, --verbose            详细
-W, --wide               不要截断IP地址
-n, --numeric            不要解析IP成域名（直接使用IP，不经过域名服务器）
--numeric-hosts          不要解析主机名
--numeric-ports          不要解析端口名
--numeric-users          不要解析用户名
-N, --symbolic           解析硬件名                      【？？？】
-e, --extend             显示更多信息
-p, --programs           显示进程ID和进程名
-c, --continuous         持续列出网络状态

-l, --listening          显示LISTENING状态的连接
-a, --all, --listening   显示所有连接(默认: 显示CONNECTED的连接)
-o, --timers             显示计时器  【netstat -to】
-F, --fib                显示转发信息库（Forwarding Information Base），默认显示
-C, --cache              显示路由缓存(routing cache)而不是FIB

-t, --tcp                显示TCP信息
-u, --udp                显示UDP信息
-w, --raw                实现raw信息
-x, --unix               显示unix信息
--ax25                   显示AMPR AX.25信息
--ipx                    显示Novell IPX信息
--ddp                    显示Appletalk DDP信息

<Socket>={-t|--tcp} {-u|--udp} {-w|--raw} {-x|--unix} --ax25 --ipx --netrom
<AF>=Use '-6|-4' or '-A <af>' or '--<af>'; default: inet
可能的地址系列列表（支持路由）:
    inet (DARPA Internet) inet6 (IPv6) ax25 (AMPR AX.25) 
    netrom (AMPR NET/ROM) ipx (Novell IPX) ddp (Appletalk DDP) 
    x25 (CCITT X.25) 
```

# 2. 示例
## 查看端口是否被占用
```bash
netstat -tuanp | grep 53
```

## 获取进程 ID/进程名
```bash
netstat -ap
```

## 显示网络统计数据
```bash
netstat -s
```

## 显示路由表
```bash
netstat -r
```

## 显示网络接口
```bash
netstat -i
netstat -ie
```

## 显示 LISTENING(监听) 状态的连接
```bash
netstat -l
```

# 3. 疑问
## masqueraded connections 是什么？

[TOC]

lsof
---

lsof: list open file（打开文件列表）。

作用：

- 查看进程打开的文件；
- 查看打开文件的进程；
- 查看进程打开的端口；
- 找回/恢复删除的文件；

> *nix 中一切皆文件

# 1.介绍

## 1.1 用法
```bash
lsof [ -?abChKlnNOPRtUvVX ] [ -A A ] [ -c c ] [ +c c ] [ +|-d d ] [ +|-D D ]
[ +|-e s ] [ +|-E ] [ +|-f [cfgGn] ] [ -F [f] ] [ -g [s] ] [ -i [i] ] [ -k k ]
[ +|-L [l] ] [ +|-m m ] [ +|-M ] [ -o [o] ] [ -p s ] [ +|-r [t[m<fmt>]] ] [ -s [p:s] ]
[ -S [t] ] [ -T [t] ] [ -u s ] [ +|-w ] [ -x [fl] ] [ -z [z] ] [ -Z [Z] ] [ -- ] [names]
```

## 1.2 选项
```bash
-a：条件与（列表选择以and为条件，默认是or）
-g：列出GID号进程详情；
-h：显示帮助信息；
-v：显示版本信息。

-c <进程名>：列出进程名以指定字符串开头的进程所打开的文件；
-d <文件号>：列出占用该文件号的进程；
+d <目录>：列出目录下被打开的文件；
+D <目录>：递归列出目录下被打开的文件；
-n <目录>：列出使用NFS的文件；
-i <条件>：列出符合条件的进程。（4、6、协议、:端口、 @ip ）
-p <进程号>：列出指定进程号所打开的文件；
-u <用户名>：列出一个或多个用户打开的文件
-r <秒数>：重复，每隔多少秒重复一次
-t <file>: 列出打开某文件的所有进程ID
```


# 2.示例
## 列出所有打开的文件
```bash
lsof
```
如果需要过滤，使用`grep`


## 查看端口是否被占用
```bash
sudo lsof -i:53
```

## 显示使用文件的进程
```bash
lsof /path/to/file1
lsof /path/to/file1 /path/to/file2
```
## 显示某个设备中所有打开的文件
```bash
lsof /dev/hd4
```

## 显示一个或多个用户打开的所有文件
```bash
lsof -u isshe
lsof -u ^isshe # 非isshe的
lsof -u isshe, pete
lsof -u isshe -u pete
```

## 列出指定进程打开的文件
```bash
lsof -p 123,456
lsof -p 123,456 -u 6464,ashley  # 默认是or
```

## 列出指定 PID 和协议类型 (IPV4/6) 打开的文件
```bash
lsof -i 4 -a -p 1234
```

## 只列出 IPV6 的网络文件 (有些需要 sudo 才能显示)
```bash
sudo lsof -i 6
sudo lsof -i
```

## 查看以某字符串开头的进程打开的文件
```bash
lsof -c apache
```

## 列出指定域名打开的文件
```bash
lsof -i @ss64.com
lsof -i @ss64.com:513-515   # 指定端口
```

## 定时显示
```bash
lsof -i :80 -r 2
```

## 列出具有特殊值 mem 的内存映射文件
```bash
lsof -d mem
```

## 列出加载到内存中并使用特殊值 txt 执行的程序
```bash
lsof -d txt
```

## 列出打开指定文件的进程
```bash
lsof -t /u/abe/bar
```

## 发送指定信号给打开指定文件的进程
```bash
kill -HUP 'lsof -t /u/abe/bar'
```

## 列出文件的打开信息
```bash
lsof /dev/log
```

## NFS 相关
要在名为`/nfs/mount/point`且服务器不可访问的 NFS 文件系统上查找具有打开文件的进程
```bash
lsof -b /nfs/mount/point
lsof -bw /nfs/mount/point   # 禁用警告信息
```

## 忽略设备缓存文件
```bash
lsof -Di
```

## 找 IPv4 套接字文件
```bash
lsof -i@128.210.15.17
```

## 找 IPv6 套接字文件
```bash
lsof -i@[0:1:2:3:4:5:6:7]
lsof -i@[::1]
```

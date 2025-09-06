[TOC]

## 命令列表

- chrt：查看或修改进程的调度策略、调度优先级。

TODO：补充完整

## TODO

- brctl
- ethtool
- fuser
- iperf3
- pmap
- iptables
- ldd
- ldconfig

## 新文档模板

详见：[模板](./TEMPLATE.md)

## 常用命令

### 查找指定后缀文件
```
find <path> -name "*.suffix"
```

### 查找文件中指定内容
```
grep -rs "xxx" <path>
```

### 指定后缀文件找到指定内容
```
find ./ -name "*.log" | xargs grep "hello world"
```

### 去掉^M
```
fromdos $file
```

### 编码转换 ISO-8894-1 -> UTF-8
```
iconv -f GB2312 -t UTF-8 input.file -o output.file
```
### 去除文件行末的空格和 table
```
sed 's/[ \t]*$//g' <filename>
\t: table
```
### 去除文件行末的空格和 table
```
sed 's/^[ \t]*//g' <filename>
\t: table
```

### 递归修改某目录下某类型文件的权限
```
给当前目录下所有 .sh 文件添加可执行权限
 find ./ -name "*.sh" | xargs chmod +x
```

### 删除目录下所有指定后缀文件
* 删除`.o`为例
```
find ./ -name "*.o" | xargs rm
find ./ -name "*.o" | xargs rm -rf
```

### 查看端口是否被占用
```bash
sudo lsof -i:53
netstat -tuanp | grep 53
fuser 80/tcp
```

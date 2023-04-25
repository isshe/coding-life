[TOC]

# 死循环
```
while true
do
done
```

# 判等
* 字符串
```
if [ "$a" == "$b" ]then if
```
* 数字
```
if [ $a -eq $b ]then if
```

# 判断文件夹是否存在(-d)/判断文件是否存在(-f)
```
if [ -d $path ]          # if [ ! -d $path ]
then
fi
```

# 获取时间
```
cur_time=`date +%s`
年月日时分秒：
date +%Y%m%d%H%M%S
```

# 输出带背景颜色
```
echo -e "\033[45;37m 紫色 \033[0m"
```
# 遍历文件夹
* 方法1：（这里的file是完整路径）
```
for file in ./*.txt
do
...
done
```
* 方法2：
注意：此方法无法处理空格！
```
for file in `ls $1` （这里file只是文件名）
do
...
done
```

# 获取自身进程ID
```
mypid=$$
```

# 从路径中获取文件名
```
file_name=${file_path##*/}
```

# 从路径中获取路径
```
	path=${file_path%/*}
```

# 一行行读文件并赋值给不同的变量
```
cat ./change_files.txt | while read action filename
do
	echo "$action + $filename"
done
```

# 获取脚本所在目录
```
basepath=$(cd `dirname $0`; pwd)
```

# 排序、去重
```
ls abc/*/softwares/*.json | xargs -n 1 basename | sort -n | uniq
```

# 字符串替换
```shell
sed -i "s/$src/$dst/g" $file
```

# 遍历
```shell
# 获取到ip后，应该是一个列表，这里需要循环一下
for((i=0;i<${#deploy_hosts[@]};i++))
do
    host_ip=${deploy_hosts[i]}
done
```

```shell
for module in ${need_start_modules[@]}; do
    start_service "${install_dir}" "${module}" $host_ip
done
```

# 参数处理

* `$0`: 当前脚本的文件名
* `$n`: 传递给脚本或函数的参数。n 是一个数字，表示第几个参数。例如，第一个参数是$1，第二个参数是$2。
* `$#`: 传递给脚本或函数的参数个数。
* `$*`: 传递给脚本或函数的所有参数。
* `$@`: 传递给脚本或函数的所有参数。被双引号(" ")包含时 `$*` 将所有参数作为整体，`$@` 还是分开的多个参数。
* `$?`: 上个命令的退出状态，或函数的返回值。
* `$$`: 当前Shell进程ID。对于 Shell 脚本，就是这些脚本所在的进程ID。

```shell
# 循环遍历参数
for((i = 1;i <= nargs; i++));
do
    name=${!i}
    echo "$name"
done

# 把参数变成变量
for i in $@; do
    eval $i="y"
done

echo $admin # output: y
echo $db    # output: y

# 去掉头部的一个参数，$@ 的值会跟随变化
shift
echo $@
```

# 数组去重

```
arr=($(awk -v RS=' ' '!a[$1]++' <<< ${arr[@]}))
```

# 只删除文件名小于某文件名的文件

```
ls dir | awk '{if ($0 <= "filename.t" ) print "dir/"$0}' | sort | xargs rm -f
```

# 只移动文件名小于某文件名的文件

```
ls dir | awk '{if ($0 <= "filename.t" ) print "dir/"$0}' | sort | xargs -I {} mv {} new_dir/
```

- 完整脚本

```
#!/bin/bash

file="$1"
new_dir="$2"

if [ ! -d "$new_dir" ]; then
    mkdir -p "$new_dir"
fi

old_dir=$(dirname $file)
filename=$(basename $file)

ls $old_dir | awk '{if ( $0 <= "'"$filename"'" ) print "'"$old_dir/"'"$0}' | sort | xargs -I {} mv {} $new_dir
```

命令中，把 awk 的参数分成了 5 段，使用 `'（单引号）`进行拼接。

# 移除文件后缀名

```
mv $file ${file%.*}
```

`${variable%pattern}` 是一种 bash shell 的字符串操作符，表示将变量 `${variable} `的值从右到左与指定的模式pattern进行匹配，并删除最短匹配的内容，返回剩余部分。

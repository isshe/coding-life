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




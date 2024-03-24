
[TOC]

perl 教程
---

有不懂优先看这个：
http://perl.linuxtoy.org/flow-control.html

已经过了一遍：
https://www.runoob.com/perl/perl-intro.html


# 注释
```perl
# 单行注释

=pod
多行注释
=cut
```

# 特殊变量
见：https://www.runoob.com/perl/perl-special-variables.html

# 数据类型
基本的数据类型：标量、数组、哈希。
标量 $ 开始，如$a $b 是两个标量。
数组 @ 开始，如 @a @b 是两个数组。
哈希 % 开始， %a %b 是两个哈希。

# 文件访问模式
|模式 | 描述|
|---|---|
|< 或 r	| 只读方式打开，将文件指针指向文件头。|
|> 或 w	| 写入方式打开，将文件指针指向文件头并将文件大小截为零。如果文件不存在则尝试创建之。|
|>> 或 a|写入方式打开，将文件指针指向文件末尾。如果文件不存在则尝试创建之。|
|+< 或 r+|读写方式打开，将文件指针指向文件头。|
|+> 或 w+|读写方式打开，将文件指针指向文件头并将文件大小截为零。如果文件不存在则尝试创建之。|
|+>> 或 a+|读写方式打开，将文件指针指向文件末尾。如果文件不存在则尝试创建之。|

# 正则表达式
* 匹配：`m//`, 可以简写为`//`
* 替换：`s///`，格式：`s/PATTERN/REPLACEMENT/`
* 转化：`tr///`

## 匹配修饰符
* i: 忽略模式中的大小写
* m: 多行模式
* o: 仅赋值一次
* s: 单行模式，"."匹配"\n"（默认不匹配）
* x: 忽略模式中的空白
* g: 全局匹配
* cg: 全局匹配失败后，允许再次查找匹配串

## 匹配完后的特殊变量
* $`: 匹配部分的前一部分字符串
* $&: 匹配的字符串
* $': 还没有匹配的剩余字符串

## 替换修饰符
* i：如果在修饰符中加上"i"，则正则将会取消大小写敏感性，即"a"和"A" 是一样的。
* m：
* o：表达式只执行一次。
* s：
* x：如果加上该修饰符，表达式中的空白字符将会被忽略，除非它已经被转义。
* g：替换所有匹配的字符串。
* e：替换字符串作为表达式

## 转化修饰符
* c: 转化所有未指定字符
* d: 删除所有指定字符
* s: 把多个相同的输出字符缩成一个

## 更多正则表达式规则
* 见：https://www.runoob.com/perl/perl-regular-expressions.html

# 面向对象
* 后缀名：`pm`
* 析构：定义 DESTROY 函数
```perl
sub DESTROY{
    # do something
}
```
* 继承：`@ISA`
```perl
@ISA = qw( 父类名称 );
```
* 重载：使用继承，然后定义重名函数即可。


# 良好的 Perl 开发习惯

## 开启内建警告信息

```perl
# 1.
perl -w my_program

# 2.
#!/usr/bin/perl -w

# 3. 可部分打开
#!/usr/bin/perl
use warnings
...
no warnings
```

## 查看更详细的问题描述

```
use diagnostics
```

## 开启严格模式

```
use strict
```

有以下好处：
* 避免拼写错误
* 限定变量的范围


## 使用帮助文档 perldoc

```
perldoc
perldoc -f print
perldoc perlsyn
perldoc perl
```

# 数组
## 特殊索引值
* `$#<array>`: 返回最后一个元素的索引值，示例如`$#arr`。
* 负数索引：从后往前索引，示例如`@arr[-1]`

# 引用

取地址：`\$var`

## 软引用

软引用：存储变量名

```perl
$scalar = "Hello1"
@scalar = ("Hello2", "World", 123)
$refer = "scalar"   # 引用变量名
$$refer             # Hello1
$$refer[0]          # Hello2
```

## 硬引用

硬引用：存储地址

``` perl
$foo = "hello";
$scalarref = \$foo;     # 标量引用
$constref = \123.456;   # 常量引用
$arrayref = \@arr;      # 数组引用
$arrayref2 = [1, 2, 3]  # 匿名数组的引用
%hash = (two => 2, three => 3, four => 4, five => 5);
$hashref = \%hash;
```

解引用

```perl
${$scalarref}   # 简写 $$scalarref
${$constref}    # 简写 $$constref
${$arrayref}[0]
@arr = @$arrayref;
${$arrayref2}[0]
%hash = %$hashref;
```

# 哈希
## 哈希函数
* exist
* each
* ENV
* reverse
* delete
* keys
* values


# 多行文本
```perl
my $page = <<HERE;
<html>
    <head><title>$title</title></head>
    <body>This is a page.</body>
</html>
HERE

my $page = <<_EOC_;
<html>
    <head><title>$title</title></head>
    <body>This is a page.</body>
</html>
_EOC_
```


# 高频用法
## 获取脚本所在的目录

```perl
use File::Basename;
use Cwd qw(abs_path);

my $basepath = dirname(abs_path(__FILE__));
```
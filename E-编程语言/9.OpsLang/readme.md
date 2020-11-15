
[TOC]

OpsLang
---

* 主Ops程序文件必须至少包含一个`goal`声明。
* 动作语句间用的是`,(逗号)`分割，最后用`;(分号)`结尾；变量定义声明用`;(分号)`结尾。


# 数组操作
* push: 往数组末尾加元素。
* pop: 从数组末尾弹出元素。（弹出：返回并删除）
* shift: 从数组头部弹出元素。
* unshift: 在数组头部添加元素。
* elems: 获取数组长度。

详见示例：[array.ops](./examples/3.variable-array.ops)

# 多行
* 使用的是和 Lua 相同的样式：`[[]]`、`[=[]=]`等。

# `$`
`$`是以下代码的语法糖。
```perl
std.send-cmd(sh/CMD/),  # the actual quote character does not matter here
stream {
    found-prompt => break;
}
```

# 正则表达式
关键字：`rx`。
* 选项
`i`: 大小写不敏感。
`x`: 空格无效化。
`s`:

* 示例
```
rx{ hello, \w+}
rx/ [0-9]+ /
rx( [a-zA-Z]* )
rx[ [a-zA-Z]* ]
rx" \d+ - \d+ "
rx' ([^a-z][a-z]+) '
rx! ([^a-z][a-z]+) !
rx# ([^a-z][a-z]+) #
rx:i/hello/
```

# 通配符
关键字：`wc`。

```
wc{foo???}
wc/*.foo.com/;
wc(/country/??/);
wc"[a-z].bar.org"
wc'[a-z].*.gov'
wc:i/hello/
```

# 引号词(Quoted Words)
关键字：`qw`。

```
qw/ foo bar baz /
等于
("foo", "bar", "baz")
```

# 布尔值
`false` 的情况：
```
0
"0"
false()
""
空列表/数组
空哈希表
```

# 注释
`#`: 单行注释
```
# 多行注释
#`(
    注释内容
)
```

# 异常
异常分两种：标准、用户自定义。
标准异常：
* timeout: 读取或写入终端仿真器（tty）超时。
* failed: 上一个（shell）命令失败，退出代码非零。
* found-prompt: 上一个（shell）命令已完成（与退出代码无关）。请注意，此异常永远不会`throws`，因为它是非致命的。
* error: 在读取或写入终端仿真器（tty）时会发生一些奇怪的错误，不包括timout和closed例外情况。
* closed: 与终端仿真器（tty）的连接过早关闭。
* too-mach-out: 从终端仿真器（tty）中读取的数据过多。可以通过 命令行实用程序`--max-out-buf-size SIZE`的opslang命令行选项来调整此限制。
* too-many-tries: 将跳转，重做或其他一些重试的调用尝试次数太多。

# 运算符优先级
```perl
# Precedence          Operators
0                   post-circumfix [], {}, <>
1                   **
2                   unary +/-/~, as
3                   * / % x
4                   + - ~
5                   << >>
6                   &
7                   | ^
8                   unary !, > < <= >= == !=
                    contains contains-word prefix suffix
                    !contains !contains-word !prefix !suffix
                    eq ne lt le gt ge
9                   ..
10                  ?:
```

* 算术运算符
```
**      power
*       multiplication
/       division
%       modulo
+       addition
-       subtraction
```

* 字符串运算符
```
x       repeat a string for several times and concatenate them together
~       string concatenation
```

* 位运算符
```
<<          shift left
>>          shift right
&           bit AND
|           bit OR
^           bit XOR
```

* 关系运算符
```perl
# 针对操作数
>           greater than
<           less than
<=          less than or equal to
>=          great than or equal to
==          equal to
!=          not equal to

# 针对字符串
gt          greater than
lt          less than
le          less than or equal to
ge          great than or equal to
eq          equal to
ne          not equal to

# 字符串模式匹配
contains            holds when the right hand side operator is *contained* in
                    the left hand side operator

contains-word       holds when the right hand side operator is *contained* as
                    a word in the left hand side operator

prefix              holds when the right hand side operator is a *prefix* of
                    the left hand side operator

suffix              holds when the right hand operator is a *suffix* of the
                    left hand side operator
```

* 范围运算符
```
1 .. 5          # equivalent to 1, 2, 3, 4, 5
'a'..'d'        # equivalent to 'a', 'b', 'c', 'd'
```

* 三元运算符
```
$a < 3 ? $a + 1 : $a
```

* 下标运算符
`[]`: 针对数组
`{}`: 针对哈希
`<>`: 针对哈希。

```
%scores<john> == %scores{"john"}
```

当前进度： rules

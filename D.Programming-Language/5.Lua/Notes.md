[TOC]

# 注意集
此文件记录Lua语言的一些需要`注意的地方`，以及一些`小技巧`、`原则`之类的。

## 1.Lua基础
* `return`语句智能是代码块的最后一句，活着是`end/slse/until`之前的最后一句。
    * 其他位置可以使用`do return end`来提前返回！
* `goto`的一些限制：【5.3支持，5.1不支持，5.2不清楚】
    * goto标签遵循可见性规则，因此不能直接跳转到一个代码块中的标签。
    * goto不能跳转到函数外。
    * goto不能跳转到局部变量的作用域。
        * 局部变量的作用域终止于声明变量的代码块中的最后一个有效(non-void)语句。
        * goto标签是无效(void)语句。
        * 详见《Lua程序设计 第4版》第85页。
* 当只需要一个返回的时候，可以套一层`()`，来实现：
```
print((string.gsub("hello, up-down!", "%A", "."))), 返回： hello..up.down.
print(string.gsub("hello, up-down!", "%A", ".")), 返回：hello..up.down. 4(4是替换次数)
```

## 2.IO操作
* 使用`io.write(a, b, c)` 而不用`io.write(a .. b .. c)`。
* 原则：只在"用后即弃"或调试代码中使用`print()`，当需要完全控制输出时，使用`io.write()`。
    * `print()`只能用于标准输出、会添加一些制表符/换行符、会自动为参数调用tostring（因此可能导致一些诡异的Bug）。
* 选项`l`是io.read()的默认参数。
* `io.read(0)`可用于测试是否到达文件尾。达到返回`nil`，否则返回`""`。
* `io.lines("filename")`会自动关闭文件；`fd:lines()`则需要手动关闭`fd`。

## 3. 时间和日期
* 相关函数不包含时区，因此应用程序需要负责解析相应的时区，才能正确使用。
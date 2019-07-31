
[TOC]

# Linux内核

## 1. 基础
### 向量/变长数组
向量：数据结构末端包含一个可选的区块。如：
```
struct abc {
    int age;
    char name[20];
    ...
    char place_holder[0];
}
```
* place_holder: 大小为0的向量。
    * 当存在可选区块时，place_holder指向此区块的起始处。
    * 当不存在可选区块时，place_holder是指向结构的尾端的指针。
    
### 捕获BUG
* `BUG_ON`: 输入条件为`假`时，打印错误信息，然后内核`panic`。
* `BUG_TRAP`: 输入条件为`假`时，打印警告信息(warning message)。

### do_something和__do_something
do_something：是__do_something的包裹函数。增加了额外的合理性检查或者加上同步/加锁处理。
__do_something：实际做事的函数。

### parse_arg
parse_arg()是一个函数，用于解析输入字符串。输入字符串的格式为`key=value`，寻找特定关键字，并启用适当的处理函数。
相关文件：kernel/params.c。

### 注册关键字/参数
内核组件可以利用`__setup`宏注册关键字和相关联的处理函数。
```c
# include <linux/init.h>

#define __setup(str, fn)    __setup_param(str, fn, fn, 0)
#define early_param(str, fn)    __setup_param(str, fn, fn, 1)
```

## 2.注意
* 内核参数已逐渐从/proc中移出，移进一个名为/sys的目录。


# A.TODO
* 进程的实现？
* 进程的调度？

**可与`J-操作系统(实现)`同步进行，一个理论，一个实例分析！**

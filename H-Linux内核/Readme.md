
[TOC]

# Linux内核

## 1. 基础
### 1.1 向量/变长数组
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

# A.TODO
* 进程的实现？
* 进程的调度？

**可与`J-操作系统(实现)`同步进行，一个理论，一个实例分析！**
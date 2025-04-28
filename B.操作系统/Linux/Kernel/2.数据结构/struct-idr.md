# struct idr —— 用于 PID 管理的数据结构

## 目的

- 了解基数树是如何动态拓展的：例如原来 3 层如何拓展成 4 层的？还是说一开始就会是 4 层？

## 草稿


```c
struct idr {
	struct radix_tree_root	idr_rt;
	unsigned int		idr_base;
	unsigned int		idr_next;
};
```


## 关联文档

- [《深入理解 Linux 进程与内存 —— 第 3 章 进程线程对比》](../Books/深入理解Linux进程与内存/第3章-进程线程的对比.md)

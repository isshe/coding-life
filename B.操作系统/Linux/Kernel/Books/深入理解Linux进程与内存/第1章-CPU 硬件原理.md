# CPU 硬件原理

## 问题

- 你知道如何查看 CPU 中的 L1、L2、L3 等缓存的大小吗？
- 为什么内存对齐后访问性能更高？
- 如何获取内存页大小？
- TLB 缓存是什么？如何提高它的命中率？有无 TLB 访问物理内存分别是怎样的？

## 查看 CPU 的 L1、L2、L3 缓存

CPU 信息所在目录：

```bash
cd /sys/devices/system/cpu/
```

查看缓存信息

```bash
# 缓存级别，index 0 和 index 1 通常是 L1，一个数据缓存、一个指令缓存
cat cpu0/cache/index0/level

# 缓存大小
cat cpu0/cache/index0/size

# 缓存类型
cat cpu0/cache/index0/type

# 缓存被哪些 CPU 共享，共享情况是怎样的
cat cpu0/cache/index3/shared_cpu_list
act cpu0/cache/index3/shared_cpu_map

# 缓存行大小，缓存并不是按字节取的，而是按指定数量的字节
cat cpu0/cache/index3/coherency_line_size
```

dmidecode 也可以查看缓存信息

```
dmidecode -t cache
```

## TLB 是什么？如何查看 TLB？

Translation Lookaside Buffer，是 CPU 缓存的一种专门用于加快虚拟地址到物理地址转换速度的**缓存**。

查看 TLB 信息：

```bash
# yum install cpuid 或 apt install cpuid
cpuid
```

## 总结

- 如何查看 CPU 工作频率？CPU 频率是恒定不变的吗？

    答：通过 /proc/cpuinfo 查看。有的 CPU 可以睿频，所以工作频率是会变的。

- top、nproc 等命令输出的核心数量是服务器上的物理核心数量吗？

    答：不一定，超线程开启时，核心数量是逻辑核心数量，是物理核心数量的两倍。

- 如何查看 CPU 中的 L1、L2、L3 等缓存的大小？

    答：通过 sysfs 查看。路径：/sys/devices/system/cpu/。

- 为什么内存对齐后访问性能更高？

    答：CPU 取内存/缓存数据时，通常不是按字节，而是按 Cache Line 进行，一般是 64 字节。就算取 1 字节，CPU 也会一次取一个 cache line。假如取 64 字节，对齐后，取数据时，只需要一次内存 IO，不对齐就需要 2 次内存 IO。

- 如何获取内存页大小？

    答：`getconf PAGE_SIZE`，通常是 4 KB，也就是 4096 字节。

- TLB 是什么？如何提高命中率？有无 TLB 访问物理内存分别是怎样的？

    答：Translation Lookaside Buffer，专门用于加快虚拟地址到物理地址转换速度的**缓存**。
        使用更大的页大小，减少页表项数量，进而提高命中率；多级 TLB；页表预取：CPU 在加载页表时，预取更多表项。
        有 TLB：虚拟地址  ->  TLB  (命中)  ->  物理地址，1 次内存访问。
        无 TLB（未命中 TLB）：虚拟地址  ->  PML4  ->  PDPT  ->  PD  ->  PT  ->  物理地址，5 次内存访问。前面四次是 4 级页表。

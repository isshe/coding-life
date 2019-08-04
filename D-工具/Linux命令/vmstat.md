[TOC]
# vmstat
报告虚拟内存统计信息：进程，内存，分页，块IO，陷阱(traps)、磁盘和cpu活动。

## 用法
```bash
vmstat [options] [delay [count]]

vmstat [-f] [-s] [-m]
vmstat [-S unit]
vmstat [-d]
vmstat [-p disk partition]
vmstat [-V]
```

## 选项
```bash
delay  更新之间的延迟，以秒为单位。如果未指定延迟，则仅打印一个报告：自引导以来的平均值。

count  更新次数。如果未指定计数且指定了延迟(delay)，则count默认为无穷大。

-a, --active
      显示活动和非活动内存（2.5.41及之后的版本）

-f, --forks
      显示自引导以来的fork数。
      这包括fork，vfork和clone系统调用，并且是相当于创建的任务总数。
      每个进程由一个或多个任务表示，具体取决于线程用法。此显示不重复（也就是只显示一次）。

-m, --slabs
      显示slabinfo。【相关字段含义见下面部分】

-n, --one-header
      标题仅显示一次而不是定期显示。

-s, --stats
      显示各种事件计数器和内存统计信息。此显示不重复（也就是只显示一次）。

-d, --disk
      报告磁盘信息（2.5.70及之后版本）

-D, --disk-sum
      报告一些有关磁盘活动的摘要统计信息。

-p, --partition device
      分区的详细统计信息。 (2.5.70及之后版本).

-S, --unit character
      更换输出的单位：1000(k)，1024(K)，1000000(m)，1048576(M) 字节。
      注意，这不会改变交换(si/so)或块(bi/bo)字段。

-t, --timestamp
      为每一行附加时间戳。

-w, --wide
      宽屏输出模式（对于具有较高内存量的系统非常有用，其中默认输出模式会受到不必要的列断裂影响）。
      输出宽度会超过每行80个字符。

-V, --version
      显示版本信息并退出。

-h, --help
      显示帮助信息并退出。

```

## 字段描述
### VM 模式字段描述
* 进程
    * r: 可运行进程的数量 (正在运行+就绪状态)。
    * b: 处于不可中断睡眠的进程数量。
* 内存
    * swpd: 使用的虚拟内存量。
    * free: 空闲内存量。(idle)
    * buff: 用作缓冲区(buffer)的内存量。
    * cache: 用作缓存(cache)的内存量。
    * inact: 非活动内存量。（-a选项）
    * active: 活动内存量。（-a选项）
* 交换内存
    * si: 从磁盘交换的内存量（/s）。
    * so: 交换到磁盘的内存量（/s）。
* IO
    * bi: 从块设备接收的块数（blocks/s）。
    * bo: 发送到块设备的块数（blocks/ s）。
* 系统
    * in: 每秒的中断数，包括时钟。
    * cs: 每秒上下文切换次数。
* CPU
    * 这些是总CPU时间的占比。
    * us: 运行非内核代码所花费的时间。  (用户时间, 包括nice时间)
    * sy: 运行内核代码所花费的时间。  (系统时间)
    * id: 空闲时间。在2.5.41版本之前，这包括IO等待时间。
    * wa: 等待IO的时间。在2.5.41版本之前，包含在空闲状态。
    * st: 从虚拟机中窃取的时间。在Linux 2.6.11之前，未知。【？？？】

### DISK 模式字段描述
* 读(reads)
    * total: 完全成功的读总数（次数还是字节？应该是次数，因为换单位(K或M)，数值也不改变）。
    * merged: 分组(groups)读取(导致一个I/O)。
    * sectors: 成功读取的扇区数。
    * ms: 花在读上的`毫秒`数。
* 写(writes)
    * total: 完全成功的写总数。
    * merged: 分组写 (导致一个I/O)。
    * sectors: 成功写的扇区数。
    * ms: 花在写上的`毫秒`数。
* IO(input/output)
    * cur: 正在处理的IO。
    * s: 花在I/O上的`秒`数。

### DISK PARTITION 模式字段描述
* reads: 发送到此分区上的读的总数。
* read sectors: 分区上读的扇区总数。
* writes: 发送到此分区上的写的总数。
* requested writes: 此分区发出的写请求总数。

### SLAB 模式字段描述
* cache: 缓存名称。
* num: 当前活动对象数量。
* total: 有效/可用对象总数。
* size: 每个对象的大小。
* pages: 具有至少一个活动对象的页面数。

## 示例
### 显示内存、CPU、中断、fork等总览信息
> vmstat -s

### 保持更新固定次数 
> vmstat -a 2       // 每2s更新一次，一直更新
> vmstat -a 1 100   // 每1s更新一次，更新100次

### 显示启动以来的fork数量
> vmstat -f

### 显示某个分区统计信息
> vmstat -p sda1

### 更换输出单位
> vmstat -s -S M

### 显示slab内存对象信息
> vmstat -m

## 疑问
* st: 从虚拟机中窃取的时间？
* 磁盘分区的相关字段不理解？

## 参考
* Linux man page
[TOC]

第二章 安装Kafka
---


# 2.6 Kafka集群
## 2.6.4 操作系统调优
* 虚拟内存
    * 主要设置以下几个参数，原因见《Kafka权威指南》26页。
        * vm.swappiness设置为较小的值，如1。
        * vm.dirty_background_ratio: 允许的脏页数量，超过会触发后台进程进行清理。值是系统内存的百分比，建议5。
        * vm.dirty_ratio: 允许的脏页数量，超过会阻塞IO，直到内核将脏数据刷写到磁盘。值是系统内存的百分比，建议60-80。
    * 使用`/proc/vmstat`检查脏页数量，通过观察Kafka集群实际运行期间脏页数量，来调整上述值的大小。
* 磁盘
    * 使用`EXT4`或`XFS`文件系统；
    * 设置noatime防止更新atime(最后访问时间)。
* 网络
    * 调整socket缓冲区大小：
        * net.core.wmem_default: 默认写缓冲区大小，合理的值是131072(128KB)
        * net.core.rmem_default: 默认读缓冲区大小，合理的值是131072(128KB)
        * net.core.wmem_max: 最大写缓冲区大小，合理的值是2097152(2MB)
        * net.core.rmem_max: 最大读缓冲区大小，合理的值是2097152(2MB)
    * 调整TCP socket的读写缓冲区：
        * net.ipv4.tcp_wmem: 由`最小值 默认值 最大值`组成；最大值不能大于net.core.wmem_max。
        * net.ipv4.tcp_rmem: 由`最小值 默认值 最大值`组成；最大值不能大于net.core.rmem_max。
    * 设置TCP时间窗口拓展：
        * net.ipv4.tcp_window_scaling: 设置为1。可以提升客户端传输数据的效率，传输的数据在服务器端进行缓存。
    * 设置并发连接数：
        * net.ipv4.tcp_max_syn_backlog: 默认1024。
    * 设置网络设备最大并发：
        * net.ipv4.netdev_max_backlog: 默认1000。设置更大的值有助于应对网络流量的爆发。


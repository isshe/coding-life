[TOC]

# 接收数据
描述设备接收一个包的过程。总体过程 + 内核函数调用流程。

# 问题
* 数据包如何从物理网卡到驱动/内核？
    * 通过中断？哪个是最初的函数？
* Linux 内核中整个流程是怎么样的？



# A.参考

* https://blog.csdn.net/lishanmin11/article/details/77162070
* https://blog.packagecloud.io/eng/2016/06/22/monitoring-tuning-linux-networking-stack-receiving-data/
* https://blog.packagecloud.io/eng/2016/10/11/monitoring-tuning-linux-networking-stack-receiving-data-illustrated/
* https://blog.packagecloud.io/eng/2017/02/06/monitoring-tuning-linux-networking-stack-sending-data/

# TODO
完成这个后，继续学习`发送数据`
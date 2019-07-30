
# 设备初始化
记录一些设备初始化的信息。
> 由于是根据《深入理解Linux网络技术内幕》学习，因此相关设备是"网络设备"，不过相关流程应该具有一定通用性。

注册和初始化的任务一部分由内核完成，一部分由设备驱动程序完成。主要分为以下几个阶段：
* 硬件初始化
* 软件初始化
* 功能初始化

设备与内核交互方式：
* 轮询（polling）：由内核端驱动。
* 中断（interrupt）：由设备端驱动。

## 1.资源分配
设备驱动程序与内核通信的资源：
* IRQ线：设备用中断来通知内核一些事情。
    * 虚拟设备不需要分派一个IRQ，如回环设备。
    * 每个中断事件都会运行一个中断处理例程(interrupt handler)。
    * 相关函数：request_irq()/free_irq()
* I/O端口和内存注册：通常是将设备内存映射到系统内存，以方便读写。
    * 相关函数：request_region()/release_region()

## 2.虚拟设备分类
* 绑定（bonding）：绑定一组物理设备，使其如同单一设备。
* 802.1Q: 以VLAN报头扩充802.3/Ethernet帧头，因而得以建立VLAN。
* 桥接（bridging）：网桥的虚拟代表。
* 别名接口(aliasing interface): 支持单物理接口多IP。（现在代码改进，不用这个也能实现多IP）
* 普通均衡器（true equalizer, TEQL）: 这是队列规则，用于流量控制。其实现需要建立一个特殊设备。TEQL背后的思想有点类似Bonding。
* 隧道接口（tunnel interface）：IP-over-IP(IPIP)隧道以及GRE（Generalized Routing Encapsulation，通用路由封装）协议的实现基础是虚拟设备的建立。


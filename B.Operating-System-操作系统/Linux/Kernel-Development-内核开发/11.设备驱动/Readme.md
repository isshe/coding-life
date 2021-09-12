[TOC]

# 设备驱动
NIC可用之前，相关联的net_device数据结构必须先初始化，添加至内核网络设备数据库、配置并开启。
注册/注销/开启/关闭是四个不同的操作，不要混淆。
相关讨论，主要以网络设备为主。

## 设备注册
触发网络设备`注册`的情况：
* 加载NIC设备驱动程序：内建在内核，则引导期间初始化；模块形式，则在运行期间初始化。
    * 例如，注册PCI设备驱动程序时，会导致`pci_driver->probe`被调用。
* 插入可热插拔网络设备：内核通知其驱动程序，驱动程序注册该设备。
注册流程：以ethernet设备为例，流程都是一样，只是细节不同。
> 示例可见：drivers/net/ethernet/intel/e100.c
```c
xxx_probe/module_init
    |
    |--->dev = alloc_etherdev(sizeof(driver_private_structure))
    |           |
    |           +---> alloc_etherdev(sizeof_priv, "eth%d", ether_setup)
    |                   |
    |                   |---> dev = kmalloc(sizeof(net_device)) + sizeof_prive + padding)
    |                   |---> ether_setup(dev)
    |                   |---> strcpy(dev->name, "eth%d")
    |                   +---> return(dev)
    |    ... ... ...
    |---> netdev_boot_setup_check(dev)
    |    ... ... ...
    +---> register_netdev(dev)
            |
            +---> register_netdevice(dev)
```

## 注销
触发网络设备`注销`的情况：
* 卸载NIC去而被驱动程序：模块形式的设备驱动程序被卸载，相关联的NIC都需要被注销。
    * 例如，卸载PCI设备驱动程序时，会导致`pci_driver->remove`被调用。
* 删除可热拔插网络设备。
注销流程：
```c
xxx_remove(_one)/module_exit
    |
    |---> unregister_netdev(dev)
    |        |
    |        +---> unregister_netdevice(dev)
    |---> ... ... ...
    |
    +---> free_netdev(dev)
```
* 注销总是会调用`unregister_netdevice`和`free_netdev`。
    * 有时显式调用free_netdev，有时则通过`dev->destructor`间接调用。
        * 只有少数虚拟设备的设备驱动程序采用这种方法，如，net/8021q/vlan.c

## 更多示例信息
见[net_device](../5.网络/3.net_device数据结构/Readme.md)

## A.问题
* 设备是何时以及如何在内核注册的？
* 网络设备如何利用网络设备数据库注册，并指派一个net_device结构的实例？
* net_device结构如何组织到hash表和列表，以便各种查询？
* net_device实例如何初始化？
> 一部分由内核核心完成，一部分由其设备驱动完成。
* 就注册而言，虚拟设备和真实设备有何差别？
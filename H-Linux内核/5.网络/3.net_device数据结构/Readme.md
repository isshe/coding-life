
# net_device
此数据结构存储着特定网络设备的所有信息。

## net_device结构中字段的分类
* 配置（Configuration）
* 统计数据（Statistics）
* 设备状态（Device status）
* 列表管理（List management）
* 流量管理（Traffic management）
* 功能专用（Feature Specific）
* 通用（Generic）
* 函数指针（或VFT）

## net_device结构的组织
已注册设备全局列表：
![](./register_dev_list.png)
根据设备名称和设备索引搜寻net_device的hash表：
![](./hash_about_net_device.png)

net_device结构有些值得注意的方面，如：
* 调用alloc_netdev分配net_device结构时，会把驱动程序的私有数据区块的大小传进去。
    * alloc_netdev会把私有数据附加到net_device结构中。
    * 私有数据的大小及内容随设备类型不同而不同。
* dev_base和net_device中的next指针指向net_device结构的开头，而非已分配区块的开头。
    * 开头补空白空间的大小存储在`dev->padded`中，方便内核释放内存区块。
* dev_base: 内含所有net_device实例的全局列表能够让内核轻易浏览设备。
* dev_name_head: 这是一张hash表，以设备名称为索引。
* dev_index_head: 这是一张hash表，以设备ID `dev->ifindex`为索引。
    * 存储设备ID供交叉引用。
* 最常见的查询都是通过`设备名称`和`设备ID`进行的。(就是前面说的两张hash表: dev_name_head/dev_index_head)
    * dev_get_by_name()
    * dev_get_by_index()
    * 所有查询都由`dev_base_lock`锁保护。
    * 详见：`net/core/dev.c`

## 设备状态
### 队列规则状态
每个网络设备都会被分派一种队列规则，流量控制以此实现其QoS机制。
net_device.state是流量控制所用的结构字段之一。取值详见`include/linux/netdevice.h`。
```
enum netdev_state_t {
	__LINK_STATE_START,	// 设备开启，由netif_running()检查
	__LINK_STATE_PRESENT,	// 设备存在，由netif_device_present()检查
	__LINK_STATE_NOCARRIER, //没有载波，由netif_carrier_ok()检查
	__LINK_STATE_LINKWATCH_PENDING,
	__LINK_STATE_DORMANT,
};
```

### 注册状态


### 注册状态

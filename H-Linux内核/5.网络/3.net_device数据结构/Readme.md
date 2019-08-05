
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
```c
enum netdev_state_t {
	__LINK_STATE_START,	// 设备开启，由netif_running()检查
	__LINK_STATE_PRESENT,	// 设备存在，由netif_device_present()检查
	__LINK_STATE_NOCARRIER, //没有载波，由netif_carrier_ok()检查
	__LINK_STATE_LINKWATCH_PENDING,
	__LINK_STATE_DORMANT,
};
```

### 注册状态
设备和网络协议栈之间的注册状态存储在`net_device.reg_state`字段中。
字段取值见`include/linux/netdevice.h`的`NETREG_XXX`相关。
```c
enum { NETREG_UNINITIALIZED=0, 	/* net_device结构已分配而且其内容都已清零 */
       NETREG_REGISTERED,		/* 设备已完成注册 */
       NETREG_UNREGISTERING,	/* 调用了unregister_netdevice */
       NETREG_UNREGISTERED,	/* 设备已完成注销/除名，但是net_device结构还没释放掉 */
       NETREG_RELEASED,		/* 调用了free_netdev，所有对net_device的引用到释放了，此结构可以被释放了 */
       NETREG_DUMMY,		/* dummy device for NAPI poll（用于NAPI轮询的虚拟设备） */
} reg_state:8;
```
状态转换-《深入理解Linux网络技术内幕》158页图8-4：
![net_device的注册状态机](./reggister_state.png)
* 状态的改变会用到界于`NETREG_UNINITIALIZED`和`NETREG_REGISTERED`之间的中间状态。（这些进程由`netdev_run_todo`处理）

## 分配net_device
```c
#include <linux/netdevice.h>

#define alloc_netdev(sizeof_priv, name, name_assign_type, setup) \
	alloc_netdev_mqs(sizeof_priv, name, name_assign_type, setup, 1, 1)
```
* 作用：分配net_device结构。
* 参数：
    * sizeof_priv: 私有数据大小？
    * name: 设备名称
    * name_assign_type: 
    * setup: 

相关包裹函数：

|网络设备类型|包裹函数名称|包裹函数定义|
|------|------|------|
|以太网(Ethernet)|alloc_etherdev||
|光纤分布式数据接口(FDDI)|alloc_fddidev||
|高性能并行接口(HPPI)|alloc_hippi_dev||
|令牌环(Token Ring)|alloc_trdev||
|光纤通道(Fibre Channel)|alloc_fcdev||
|红外数据标准协会(InDA)|alloc_irdadev||

## 设备注册/注销
```c
#include <linux/netdevice.h>    // 实现在net/core/dev.c中

int register_netdev(struct net_device *dev);
void unregister_netdev(struct net_device *dev);
```
* 作用：注册或注销设备。
* 参数：
    * dev: 要注册/注销的设备
* 以上两个分别是：`register_netdevice()`和`unregister_netdevice()`的包裹函数。
* 注意：
    * 设备的注册和注销都是通过`netdev_run_todo()`完成的。
    * 为设备注册/注销时，设备驱动程序可以使用`init()`和`uninit()`这两个net_device虚拟函数，分别对私有数据做初始化以及清理工作。
    * 对设备注销时，除非对相关联的net_device引用都已全部释放，否则无法完成：netdev_wait_allrefs不会反悔，直到条件满足。

### netdev_run_todo
![register_unregister_netdev](./register_unregister_netdev.png)

```c
void rtnl_unlock(void)
{
	/* This fellow will unlock it for us. */
	netdev_run_todo();
}
```
* 注册由：`register_netdevice()`和`netdev_run_todo()`共同完成，详见下图。
* 对`net_device`的修改，会受到`rtnl_lock()/rtnl_unlock()`操作的`Routing Netlink`信号量的保护。
* `register_netdevice()`完成其份内工作后，就会用`net_set_todo()`把`net_device`结构添加到`net_todo_list`
* `net_todo_list`内的设备，注册都必须完成才行。列表不会由另一个内核线程处理或通过定时定时器处理，而是由`register_netdev`在释放锁的时候，**间接处理**。
* 任何时刻，只有一个CPU可以执行`net_run_todo()`，串行化是通过`net_todo_run_mutex`互斥体实现的。
    * **`net_todo_run_mutex`在`4.19.60`中已找不到，应该是删掉了！**
* `netdev_run_todo()`处理任务时，不用持有锁，因此此函数可以安全进入休眠状态。


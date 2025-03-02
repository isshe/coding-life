
[TOC]

# sk_buff 分配与释放
数据缓冲区和报头 (sk_buff 数据结构) 是两种不同的实例。
也就是说，建立一个缓冲区会涉及到两次内存分配：
* 先分配 sk_buff 结构——从缓冲区 (cache) 中；（kmem_cache_alloc_node()）
* 再分配一块`强制对齐`的内存，存放实际的数据包（包括头部）；(kmalloc_reserve())
    * skb->head/data 指向此内存区域；
* 强制对齐示意图如下：(图 1)
```
skb->head/data/tail---> +-------+   ---     ---
                        |       |    |       |
                        |       |   size   SKB_DATA_ALIGN(size)
                        |       |    |       |
                        +-------+   ---      |
                        |填充区域|            |
skb->end -------------> +-------+           ---
                        |       |   --> struct skb_shared_info
                        +-------+
```
> struct skb_shared_info 结构中还可能指向一些片段数据/片段缓冲区。

* 一些约定：
    * skb: 指向 sk_buff 的指针。（sk_buff 缓冲区）
    * 数据：skb->head/skb->end 指向的数据缓冲区。（数据缓冲区）
    * 片段缓冲区：struct skb_shared_info 结构中指针指向的缓冲区。（片段缓冲区、片段数据、片段。）

## 1.分配
```c
#include <linux/skbuff.h>

static inline struct sk_buff *alloc_skb(unsigned int size, gfp_t priority);
static inline struct sk_buff *dev_alloc_skb(unsigned int length);
static inline struct sk_buff *netdev_alloc_skb(struct net_device *dev, unsigned int length);
```
* 关系：
    * `alloc_skb()`是`__alloc_skb()`的包裹函数；
    * `dev_alloc_skb()`是`netdev_alloc_skb()`的包裹函数；
    * 这三个函数最终都会调用`__alloc_skb()`函数；
* 作用：分配一个 skb——网络缓冲区。
    * dev_alloc_skb(): 在特定的设备上为`rx(接收)`分配一个 skbuff。由设备驱动程序使用的缓冲区分配函数，而且应该在中断模式中执行。
* 参数：
    * length/size: 分配的大小，单位字节；
    * priority：分配掩码（allocation mask）；
    * dev: 接收的网络设备；
* 返回：
    * 成功：指向一个 skbuff 的指针；
    * 失败：NULL；


## 2.释放
```c
#include <linux/skbuff.h>

void kfree_skb(struct sk_buff *skb);
#define dev_kfree_skb(a)	consume_skb(a)
void consume_skb(struct sk_buff *skb);
```
* 作用：当 skb->users==1 时，释放一个 sk_buff。consume_skb 作用和 kfree_skb 相同，但是 kfree_skb 假定在失败时帧已经被丢弃。
    * 调用 dst_release 递减 dst_entry 数据结构的引用计数；
    * 调用 destructor（如果存在的话）；
    * 释放片段所持有的内存（如果存在的话），由 skb_shared_info 数据结构持有的指针指向；
* 参数：
    * skb：指向 sk_buff 的指针。
* 返回：无。
* 流程：
```flow
st=>start: 开始
en1=>end: 结束
en2=>end: 结束
dec_users=>operation: 递减skb引用计数(skb->users)
is_users_zero=>condition: skb引用计数为0吗？
release_dst=>operation: 释放所有dst_entry
is_destructor_init=>condition: skb->destructor已经初始化了吗？
do_desctructor=>operation: 执行析构函数
is_clone_skb=>condition: skb是克隆的吗？
dec_dataref=>operation: 递减数据的引用计数
is_dataref_nozero=>condition: dataref引用计数不为0吗？
free_buff=>operation: 释放该主要缓冲区及其片段
skb_return=>operation: skb返回缓存


st->dec_users->is_users_zero
is_users_zero(no)->en1
is_users_zero(yes)->release_dst
release_dst->is_destructor_init
is_destructor_init(no)->is_clone_skb
is_destructor_init(yes)->do_desctructor->is_clone_skb
is_clone_skb(no)->free_buff
is_clone_skb(yes)->dec_dataref->is_dataref_nozero
is_dataref_nozero(no)->free_buff
is_dataref_nozero(yes)->skb_return
free_buff->skb_return
skb_return->en2
```

## 3. 数据预留及对其
```c
#include <linux/skbuff.h>

static inline void skb_reserve(struct sk_buff *skb, int len);
```
* 作用：调整头部空间（adjust headroom）、在缓冲区头部预留一些空间。
    * 通过减少尾部空间来增加空 sk_buff 的净空。（Increase the headroom of an empty &sk_buff by reducing the tail room.）
* 参数：
    * skb: 指向需要调整的 sk_buff。
    * len：调整的字节数。
* 返回：无。
* 注意：
    * 此函数只能用于空 sk_buff。
    * 此函数没有拷贝任何数据到缓冲区，只是更新了两个指针。
    * 当缓冲区往下传播进过网络协议栈时，每个协议都会把`skb->data`往下传，并将其报头拷贝进来，然后更新`skb->len`。
* 使用示例：
```c
skb_reserve(skb, 2);    // 吧 IP 对齐在 16 字节地址边界上
```
* 示意图：
    * 从下层到上层：
    ![](./skb_reserve_down_to_up.png)
    * 从上层到下层：
    ![](./skb_reserve_up_to_down.png)


## 4. skb_push/skb_put/skb_pull
```c
#include <linux/skbuff.h>

void *skb_pull(struct sk_buff *skb, unsigned int len);
void *skb_push(struct sk_buff *skb, unsigned int len);
void *skb_put(struct sk_buff *skb, unsigned int len);
```
* 作用：（以下操作都只是移动指针，不涉及数据拷贝）
    * skb_put：在尾部添加数据到 sk_buff。
    * skb_push：在头部添加数据到 sk_buff。
    * skb_pull：从头部减少 sk_buff 数据。
* 参数：
    * skb：指向要操作的 sk_buff 的指针。
    * len: 操作长度。
* 返回：无。
* 示意图：
![](./skb_put_push_pull_reserve.png)

## 5. skb_shared_info 结构和 skb_shinfo 函数
如最上面的`图1`所示，数据缓冲区尾部 (skb->end) 有一个名为`skb_shared_info`的数据结构。此结构用于保持此数据区块的附加信息。

## 5.1 结构定义
```c
/*
 * 此数据在克隆中不变，并且位于 header data 的末尾，即。在 skb->end。
 * 
struct skb_shared_info {
	__u8		__unused;
	__u8		meta_len;
	__u8		nr_frags;   // nr_flags/frag_list/frags[]在《深入理解Linux网络技术内幕》第21章讨论
	__u8		tx_flags;
	unsigned short	gso_size;
	/* Warning: this field is not always filled in (UFO)! */
	unsigned short	gso_segs;
	struct sk_buff	*frag_list;
	struct skb_shared_hwtstamps hwtstamps;
	unsigned int	gso_type;
	u32		tskey;

	/*
	 * dataref：引用计数
	 * 注意：所有 dataref 前面的字段都会被__alloc_skb() 清除。
	 */ 
	atomic_t	dataref;

	/*
	 * 中间层必须确保 destructor_arg 在 skb 析构函数之前保持有效 
	 */
	void *		destructor_arg;

	/* 必须是最后一个字段，详见 pskb_expand_head() */
	skb_frag_t	frags[MAX_SKB_FRAGS];
};
```

## 5.2 操作函数
```c
#define skb_shinfo(SKB)	((struct skb_shared_info *)(skb_end_pointer(SKB)))
```
* 作用：返回 skb->end 指针，用于指向 struct skb_shared_info 的起始地址。
* 参数：
    * SKB：指向 skb 的指针。
* 返回：skb_share_info 指针。

## 6. sk_buff 的克隆和拷贝
```c
#include <linux/skbuff.h>

struct sk_buff *skb_clone(struct sk_buff *skb, gfp_t priority);
static inline struct sk_buff *pskb_copy(struct sk_buff *skb, gfp_t gfp_mask)
struct sk_buff *skb_copy(const struct sk_buff *skb, gfp_t priority);

```
* 作用：【如下面两个图】
    * skb_clone: 只复制 sk_buff 结构，不拷贝数据和片段缓冲区。采用引用计数的方式共享数据块。(duplicate an sk_buff)
    * pskb_copy: 复制 sk_buff 结构和数据，不拷贝片段缓冲区。（create copy of an sk_buff with private head.）
    * skb_copy: 复制 skb_buff 结构、数据和片段缓冲区。(create private copy of an sk_buff)
        * 将非线性 sk_buff 转换为线性。
* 参数：
    * skb: 要拷贝的 sk_buff。
    * priority/gfp_mask: 分配优先权/掩码 (allocaiton priority)
* 返回：
    * 成功：指向 struct sk_buff 的指针。
    * 失败：NULL。
* 注意：
    * skb_clone 出来的 sk_buff 克隆没有链接到任何表（list），而且也没有引用套接字的拥有者。
        * skb->cloned 字段在克隆和原缓冲区都被置为 1；克隆的 skb->users 也被置为 1.
* 三个函数的示意图：
![](./skb_clone.png)
![](./pskb_copy_skb_copy.png)


## 7. sk_buff 列表管理函数
以下的函数会操作 sk_buff 元素列表，列表也成为队列 (queue)。
```c
#include <linux/skbuff.h>

// 初始化队列头部。（队列中的所有 skb 都有一个指向头部的指针）
static inline void skb_queue_head_init(struct sk_buff_head *list);

// 添加到队列头部
void skb_queue_head(struct sk_buff_head *list, struct sk_buff *newsk);

// 添加到队列尾部
void skb_queue_tail(struct sk_buff_head *list, struct sk_buff *newsk);

// 从队列中删除一个 skb
void skb_unlink(struct sk_buff *skb, struct sk_buff_head *list);

// 从头部删除一个 skb
struct sk_buff *skb_dequeue(struct sk_buff_head *list);

// 从尾部删除一个 skb
struct sk_buff *skb_dequeue_tail(struct sk_buff_head *list);

// 清空一个队列
void skb_queue_purge(struct sk_buff_head *list);

// 遍历队列
#define skb_queue_walk(queue, skb) \
		for (skb = (queue)->next;					\
		     skb != (struct sk_buff *)(queue);				\
		     skb = skb->next)
```
* 参数：
    * newsk: 指向新的 sk_buff 的指针。
    * list: 列表/队列。
* 返回：指向 sk_buff 的指针或者没有返回。
* 注意：以上`函数`都必须以原子方式执行，也就是说持有`sk_buff_head`结构提供的`回旋锁`。

## A. 参考
* 《深入理解 Linux 网络技术内幕》
* 《Linux Kernel 5.2.1》



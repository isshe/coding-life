
# 中断

## 1. 硬件中断注册
```c
#include <linux/interrupt.h>

static inline int __must_check
request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags, const char *name, void *dev)
```
* 作用：申请一个中断线。
* 参数：
    * irq: 中断线号
    * handler: 中断处理函数
    * flags:
    * name:
    * dev:

## 2. 硬件中断释放
```c
#include <linux/interrupt.h>

const void *free_irq(unsigned int irq, void *dev_id);
```
* 作用：释放一个中断线。如果没有其他设备注册该 IRQ 线，就关闭该 IRQ。
* 参数：
    * irq: 中断线号。
    * dev_id: 设备标识。
    
## 3. 中断类型
网络设备常见的中断事件类型：
* 接收一帧：最常见、标准的情况。
* 传输失败；
* DMA 传输已成功完成；
    * drivers/net/3c59x.c有相关范例。
* 设备有足够内存处理新传输；

## 4. 中断共享
IRQ 线是有限的资源；允许系统能容纳设备数目的简单方式就是：允许多台设备共享一个 IRQ 线。
一组设备共享一条 IRQ 线时，所有这些设备的设备驱动程序都必须有能力处理共享的 IRQ。换言之，设备注册 IRQ 时，需要说明其是否支持中断共享。

## 5. IRQ 处理函数映射（中断向量表）
相关结构：详见<include/linux/interrupt.h>
* struct irq_desc
* struct irqaction

```c
irq_desc ---->+---------+
          |   |         |
              +---------+
          |   | *action | --------------------> +---------------+
              +---------+                       |     *next     |
          |   |         |                       |               |
              |         |                       +---------------+
          |   +---------+                        struct irqaction
     NR_IRQS  | *action | --------------------> +---------------+ ------> +---------------+
          |   +---------+                       |     *next     |         |     *next     |
              |         |                       |   SA_SHIRQ    |         |               |
          |   |         |                       +---------------+         +---------------+
              +---------+ --                     struct irqaction          struct irqaction
          |   |         |   | -> struct irq_desc 
         -----+---------+ --
```


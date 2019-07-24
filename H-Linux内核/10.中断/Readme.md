
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
* 作用：释放一个中断线。如果没有其他设备注册该IRQ线，就关闭该IRQ。
* 参数：
    * irq: 中断线号。
    * dev_id: 设备标识。
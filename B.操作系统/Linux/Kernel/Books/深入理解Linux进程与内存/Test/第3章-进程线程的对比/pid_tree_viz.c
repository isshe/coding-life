#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/pid.h>
#include <linux/pid_namespace.h>
#include <linux/xarray.h>
#include <linux/sched.h>
#include <linux/rcupdate.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("isshe");
MODULE_DESCRIPTION("PID Radix Tree Visualization Module");

// 前向声明
static void examine_xa_node(void *entry, unsigned long index, unsigned int height);

static int __init pid_tree_init(void)
{
    struct pid_namespace *ns = task_active_pid_ns(current);
    struct xarray *xa;

    if (!ns) {
        printk(KERN_ERR "Failed to get active PID namespace\n");
        return -EINVAL;
    }

    xa = &ns->idr.idr_rt;

    printk(KERN_INFO "PID Radix Tree Visualization Module loaded\n");
    printk(KERN_INFO "Examining PID tree in current namespace\n");

    rcu_read_lock();

    // 从根节点开始遍历
    examine_xa_node(xa_head(xa), 0, 0);

    rcu_read_unlock();

    return 0;
}

// 深度优先搜索
static void examine_xa_node(void *entry, unsigned long index, unsigned int height)
{
    if (!entry)
        return;

    // 检查是否是内部节点
    if (!xa_is_node(entry)) {
        // 这是一个叶子节点（实际值）
        struct pid *pid = (struct pid *)entry;
        struct task_struct *task = NULL;

        if (!xa_is_value(entry) && pid) {
            task = pid_task(pid, PIDTYPE_PID);
            if (task) {
                printk(KERN_INFO "Leaf: index=%lu, PID=%d, Process=%s, TGID=%d\n",
                       index, pid_nr(pid), task->comm, task->tgid);
            } else {
                printk(KERN_INFO "Leaf: index=%lu, PID=%d (no task)\n",
                       index, pid_nr(pid));
            }
        } else {
            printk(KERN_INFO "Leaf: index=%lu, special entry\n", index);
        }
        return;
    }

    // 内部节点处理
    struct xa_node *node = xa_to_node(entry);
    unsigned int shift = node->shift;

    printk(KERN_INFO "Node: height=%u, shift=%u, offset=%u, count=%u\n",
           height, shift, node->offset, node->count);

    // 遍历子节点
    for (int i = 0; i < XA_CHUNK_SIZE; i++) {
        void *slot = rcu_dereference_raw(node->slots[i]);
        if (slot) {
            unsigned long slot_index = index | ((unsigned long)i << shift);
            examine_xa_node(slot, slot_index, height + 1);
        }
    }
}

static void __exit pid_tree_exit(void)
{
    printk(KERN_INFO "PID Radix Tree Visualization Module unloaded\n");
}

module_init(pid_tree_init);
module_exit(pid_tree_exit);

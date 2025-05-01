#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/pid.h>
#include <linux/pid_namespace.h>
#include <linux/radix-tree.h>
#include <linux/xarray.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("isshe");
MODULE_DESCRIPTION("PID Radix Tree Visualization Module");

// static void print_radix_tree_layer(struct radix_tree_root *root, int height)
// {
//     struct radix_tree_iter iter;
//     void **slot;
//     int count = 0;

//     printk(KERN_INFO "PID Tree Layer %d:\n", height);

//     radix_tree_for_each_slot(slot, root, &iter, 0) {
//         struct pid *pid = radix_tree_deref_slot(slot);
//         if (!radix_tree_exception(pid) && pid) {
//             printk(KERN_INFO "  PID: %d, nr: %d\n",
//                    iter.index, pid->numbers[0].nr);
//             count++;
//         }

//         // if (count > 100) {
//         //     printk(KERN_INFO "  ... (more entries truncated)\n");
//         //     break;
//         // }
//     }

//     printk(KERN_INFO "Total PIDs in layer %d: %d\n", height, count);
// }

static int __init pid_tree_init(void)
{
    struct pid_namespace *ns = task_active_pid_ns(current);
    struct radix_tree_root *pid_tree = &ns->idr.idr_rt;
    struct xarray *xa = (struct xarray *)pid_tree;
    unsigned long index;
    void *entry;


    printk(KERN_INFO "PID Radix Tree Visualization Module loaded\n");
    printk(KERN_INFO "Examining PID tree in current namespace\n");

    rcu_read_lock();
    for (index = 0,
         entry = xa_find(xa, &index, ULONG_MAX, XA_PRESENT);
         entry;
         entry = xa_find_after(xa, &index, ULONG_MAX, XA_PRESENT)
    ) {
        // 检查是否是内部节点
        if (xa_is_internal(entry)) {
            struct xa_node *node = xa_to_node(entry);
            if (node) {
                printk(KERN_INFO "Internal node at index %lu: shift=%d, count=%d\n",
                       index, node->shift, node->count);

                // 可以进一步打印节点的槽位信息
                int i;
                for (i = 0; i < XA_CHUNK_SIZE; i++) {
                    void *slot = rcu_dereference(node->slots[i]);
                    if (slot) {
                        printk(KERN_INFO "  Slot[%d]: %p\n", i, slot);
                    }
                }
            }
        } else {
            // 这是叶子节点，打印 PID 值
            printk(KERN_INFO "Leaf node at index %lu: PID=%lu\n",
                   index, (unsigned long)entry);

            // 如果想获取更多关于这个 PID 的信息，可以尝试获取对应的 task_struct
            struct pid *pid_struct = find_get_pid(index);
            if (pid_struct) {
                struct task_struct *task = pid_task(pid_struct, PIDTYPE_PID);
                if (task) {
                    printk(KERN_INFO "  Process: %s (comm), TGID=%d\n",
                           task->comm, task->tgid);
                }
                put_pid(pid_struct);
            }
        }

        // 确保我们继续前进
        // index++;
    }
    rcu_read_unlock();

    return 0;
}

static void __exit pid_tree_exit(void)
{
    printk(KERN_INFO "PID Radix Tree Visualization Module unloaded\n");
}

module_init(pid_tree_init);
module_exit(pid_tree_exit);

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/pid.h>
#include <linux/pid_namespace.h>
#include <linux/sched.h>
#include <linux/radix-tree.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("isshe");
MODULE_DESCRIPTION("PID Radix Tree Visualization Module");

// 递归打印基数树的辅助函数
// static void print_radix_tree_node(struct radix_tree_node *node, int level)
// {
//     int i;
//     char indent[256] = {0};

//     if (!node)
//         return;

//     // 创建缩进
//     for (i = 0; i < level * 2; i++)
//         indent[i] = ' ';

//     printk(KERN_INFO "%sNode at level %d, count: %d\n",
//            indent, level, node->count);

//     // 打印此节点的所有项
//     for (i = 0; i < RADIX_TREE_MAP_SIZE; i++) {
//         if (node->slots[i]) {
//             if (radix_tree_is_internal_node(node->slots[i])) {
//                 struct radix_tree_node *child = radix_tree_node_from_slot(node->slots[i]);
//                 printk(KERN_INFO "%s  Slot %d: Internal node\n", indent, i);
//                 print_radix_tree_node(child, level + 1);
//             } else {
//                 struct pid *pid_struct = (struct pid *)node->slots[i];
//                 printk(KERN_INFO "%s  Slot %d: PID %d\n",
//                        indent, i, pid_struct ? pid_struct->numbers[0].nr : -1);
//             }
//         }
//     }
// }

// 回调函数，用于 idr_for_each
static int print_pid_entry(int id, void *p, void *data)
{
    struct pid *pid_struct = p;
    int level = *(int *)data;

    if (!pid_struct)
        return 0;

    // 打印 PID 信息
    printk(KERN_INFO "%*s[%d] PID: %d, level: %d, ref count: %d\n",
           level * 2, "", id,
           pid_struct->numbers[0].nr,
           pid_struct->level,
           atomic_read(&pid_struct->count.refs));

    // 如果需要，可以打印更多关于此 PID 的信息
    if (pid_task(pid_struct, PIDTYPE_PID)) {
        struct task_struct *task = pid_task(pid_struct, PIDTYPE_PID);
        printk(KERN_INFO "%*s    -> Task: %s (comm)\n",
               level * 2, "", task->comm);
    }

    return 0;
}

static void print_idr_level(struct idr *idr, int level)
{
    printk(KERN_INFO "%*sIDR Level %d:\n", level * 2, "", level);
    idr_for_each(idr, print_pid_entry, &level);
}

static int __init pid_tree_print_init(void)
{
    struct pid_namespace *ns;

    printk(KERN_INFO "PID Radix Tree Visualization Module loaded\n");

    // 获取当前进程的 PID 命名空间
    ns = task_active_pid_ns(current);
    if (!ns) {
        printk(KERN_ERR "Failed to get PID namespace\n");
        return -EINVAL;
    }

    printk(KERN_INFO "PID namespace level: %d\n", ns->level);

    // 获取并打印 PID 基数树的根节点
    printk(KERN_INFO "PID Radix Tree Structure:\n");
    printk(KERN_INFO "=======================\n");

    print_idr_level(&ns->idr, 0);

    printk(KERN_INFO "Total PIDs in cursor: %d\n", idr_get_cursor(&ns->idr));
    printk(KERN_INFO "=======================\n");

    return 0;
}

static void __exit pid_tree_print_exit(void)
{
    printk(KERN_INFO "PID Radix Tree Visualization Module unloaded\n");
}

module_init(pid_tree_print_init);
module_exit(pid_tree_print_exit);

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/pid.h>
#include <linux/pid_namespace.h>
#include <linux/radix-tree.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("PID Radix Tree Visualization Module");

static void print_radix_tree_layer(struct radix_tree_root *root, int height)
{
    struct radix_tree_iter iter;
    void **slot;
    int count = 0;

    printk(KERN_INFO "PID Tree Layer %d:\n", height);

    radix_tree_for_each_slot(slot, root, &iter, 0) {
        struct pid *pid = radix_tree_deref_slot(slot);
        if (!radix_tree_exception(pid) && pid) {
            printk(KERN_INFO "  PID: %d, nr: %d\n",
                   iter.index, pid->numbers[0].nr);
            count++;
        }

        // if (count > 100) {
        //     printk(KERN_INFO "  ... (more entries truncated)\n");
        //     break;
        // }
    }

    printk(KERN_INFO "Total PIDs in layer %d: %d\n", height, count);
}

static int __init pid_tree_init(void)
{
    struct pid_namespace *ns = task_active_pid_ns(current);
    struct radix_tree_root *pid_tree = &ns->idr.idr_rt;
    int i;

    printk(KERN_INFO "PID Radix Tree Visualization Module loaded\n");
    printk(KERN_INFO "Examining PID tree in current namespace\n");

    // 打印基数树的每一层
    for (i = 0; i < 4; i++) {  // 基数树通常不会超过 4 层
        print_radix_tree_layer(pid_tree, i);
    }

    return 0;
}

static void __exit pid_tree_exit(void)
{
    printk(KERN_INFO "PID Radix Tree Visualization Module unloaded\n");
}

module_init(pid_tree_init);
module_exit(pid_tree_exit);

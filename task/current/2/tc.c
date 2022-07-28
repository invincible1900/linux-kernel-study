#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/task.h>

void tc(void){
    struct task_struct *iter;
    pr_info("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    rcu_read_lock();
    list_for_each_entry_rcu(iter, &init_task.tasks, tasks) {
       pr_info("The current task of the list is: %d, %s, %px, %px, %d\n", 
                       iter->pid, iter->comm, iter->stack, iter,
                       iter->on_cpu
                       );
    }

    pr_info("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    list_for_each_entry_rcu(iter, &current->tasks, tasks) {
       pr_info("The current task of the list is: %d, %s, %px, %px, %d\n", 
                       iter->pid, iter->comm, iter->stack, iter,
                       iter->on_cpu
                       );
    }

    rcu_read_unlock();
}

static int __init tc_init(void)
{
    pr_info("Hello tc\n");
    tc();
    return 0;
}

static void __exit tc_exit(void)
{
    pr_info("Goodbye tc\n");
}


module_init(tc_init);
module_exit(tc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("1.0");


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>

void tc(void){
        struct task_struct *pfo_ret__;
        asm("movq %%gs:%P1,%0" : "=r" (pfo_ret__) : "p"(&(current_task)));
        pr_info("%px\n", pfo_ret__);
        pr_info("%px\n", &(current_task));
        pr_info("%px, %s, %d, %px\n", current, current->comm, current->pid, current->stack);
}
/*
[ 1621.249849] ffff88806a58c600
[ 1621.251834] 0000000000015d00
[ 1621.255900] ffff88806a58c600, insmod, 1955, ffffc900004e8000
*/

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


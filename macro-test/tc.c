#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>

static void tc(void){
#ifdef CONFIG_PREEMPT_COUNT
        pr_info("CONFIG_PREEMPT_COUNT: y\n");
#else
        pr_info("CONFIG_PREEMPT_COUNT: n\n");
#endif

#ifdef CONFIG_PREEMPTION
        pr_info("CONFIG_PREEMPTION: y\n");
#else
        pr_info("CONFIG_PREEMPTION: n\n");
#endif


        return;
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


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>

static void tc(void){
#if 1
        pr_info("%d\n", preempt_count());
        preempt_disable();
        pr_info("%d\n", preempt_count());
        preempt_enable();
        pr_info("%d\n", preempt_count());
#endif
        int i;
        int cpu;
        int cpu2;
        for(i = 0; i < 100; i++){
            pr_info("[0] %d, %d\n", preempt_count(), get_cpu());
        }

        cpu = get_cpu();
        put_cpu();
        for(i = 0; i < 100000; i++){
            schedule();
            cpu2 = get_cpu();
            if(cpu != cpu2){
                pr_info("[1] %d, %d, %d\n", preempt_count(), cpu, cpu2);
                break;
            }
            put_cpu();
        }

        preempt_disable();
        cpu = get_cpu();
        put_cpu();
        for(i = 0; i < 100000; i++){
            schedule();
            cpu2 = get_cpu();
            if(cpu != cpu2){
                pr_info("[2] %d, %d, %d\n", preempt_count(), cpu, cpu2);
                break;
            }
            put_cpu();
        }
        preempt_enable();

        return;
}


static int __init tc_init(void)
{
    //pr_info("Hello tc\n");
    tc();
    return 0;
}

static void __exit tc_exit(void)
{
    //pr_info("Goodbye tc\n");
}


module_init(tc_init);
module_exit(tc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("1.0");


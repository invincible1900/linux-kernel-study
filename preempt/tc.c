#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>

void pkv(void){
    	pr_info("%d\n", preempt_count());
        preempt_disable();
    	pr_info("%d\n", preempt_count());
        preempt_enable();
    	pr_info("%d\n", preempt_count());
}


static int __init pkv_init(void)
{
    printk(KERN_INFO "Hello pkv\n");
    pkv();
    return 0;
}

static void __exit pkv_exit(void)
{
    printk(KERN_INFO "Goodbye pkv\n");
}


module_init(pkv_init);
module_exit(pkv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("0.1");


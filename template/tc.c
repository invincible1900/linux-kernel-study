#include <linux/module.h>
#include <linux/kernel.h>

static void tc(void){
        pr_info("do something\n");
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


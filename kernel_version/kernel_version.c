/*
*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>


static void pkv(void){
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,14)
	pr_info("[PKV] >=\n");
#else
	pr_info("[PKV] < \n");
#endif
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
MODULE_AUTHOR("ikx");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("0.1");

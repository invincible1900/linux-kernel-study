#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/workqueue.h>


static void pkv(void){

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


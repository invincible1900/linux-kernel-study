#include <linux/module.h>
#include <linux/kernel.h>


static void bug_on_t(void){
	/*BUG_ON(1);*/
	WARN_ON(1);
	pr_info("[D] %s-%d\n", __func__, __LINE__);
}

static int __init bug_on_t_init(void)
{
    printk(KERN_INFO "Hello \n");
    bug_on_t();
    return 0;
}

static void __exit bug_on_t_exit(void)
{
    printk(KERN_INFO "Goodbye\n");
}


module_init(bug_on_t_init);
module_exit(bug_on_t_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("0.1");

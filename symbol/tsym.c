#include <linux/module.h>
#include <linux/kallsyms.h> // kallsyms_lookup_name

static void tsym(void){
	void *addr;
	addr = (void *)kallsyms_lookup_name("do_init_module");
	pr_info("do_init_module: %px\n", addr);
}



static int __init tsym_init(void)
{
    printk(KERN_INFO "Hello tsym\n");
    tsym();
    return 0;
}

static void __exit tsym_exit(void)
{
    printk(KERN_INFO "Goodbye tsym\n");
}


module_init(tsym_init);
module_exit(tsym_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("Kernel tsym Module.");
MODULE_VERSION("0.1");

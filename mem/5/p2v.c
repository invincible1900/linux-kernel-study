#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/version.h>


long run(void *arg){
    int cpu = get_cpu();
    const char *str = "Hello";
    pr_info("%s\n", str);
    pr_info("cpu: %d\n", cpu);
    pr_info("PAGE_OFFSET: %px, v2p: %px, %px\n", PAGE_OFFSET, __pa(str), str);
    return 0;
 
}

static int __init km_init(void){
   work_on_cpu(1, run, NULL);
   return 0;
}

static void __exit km_exit(void)
{
    pr_info("Bye\n");
}

module_init(km_init);
module_exit(km_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("0.1");


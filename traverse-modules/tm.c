#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/rculist.h>


static void tm(void){
    struct module *mod = NULL;
    struct module *head_module = NULL;
    struct list_head *head = NULL;

    preempt_disable();
    list_for_each_entry_rcu(head_module, &(THIS_MODULE->list), list) {
            if(strlen(head_module->name) == 0){
                    head = &(head_module->list);
                    break;
            }
    }

    list_for_each_entry_rcu(mod, head, list) {
        pr_info(" %s, %pS, %d\n", mod->name, mod, mod->state);
    }
    preempt_enable();

}


static int __init tm_init(void)
{
    printk(KERN_INFO "Hello tm\n");
    tm();
    return 0;
}

static void __exit tm_exit(void)
{
    printk(KERN_INFO "Goodbye tm\n");
}


module_init(tm_init);
module_exit(tm_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("0.1");


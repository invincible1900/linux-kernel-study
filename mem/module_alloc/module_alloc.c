#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/version.h>
#include <linux/slab.h>
#include "kallsyms.h"

void *( *module_alloc)(unsigned long size);

long run(void){
    char stack_mem[100];
    char *mod_alloc_mem; 
    char *kalloc_mem;

    init_kallsyms();
    module_alloc = (void *)kallsyms_lookup_name("module_alloc");

    mod_alloc_mem = module_alloc(100);
    kalloc_mem = kzalloc(100, GFP_KERNEL);


    pr_info("stack_mem: %px\n", stack_mem);
    pr_info("mod_alloc_mem: %px\n", mod_alloc_mem);
    pr_info("kalloc_mem: %px\n", kalloc_mem);
    pr_info("func run: %px\n", (void *)run);

    return 0;
 
}

static int __init km_init(void){
   run();
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


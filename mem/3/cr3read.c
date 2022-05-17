#include <linux/module.h>
#include <linux/kernel.h>

/*
 * [CR3](https://en.wikipedia.org/wiki/Control_register#CR3): 
 *
 * Used when virtual addressing is enabled, 
 * hence when the PG bit is set in CR0. CR3 enables the processor 
 * to translate linear addresses into physical addresses 
 * by locating the page directory and page tables for the current task. 
 *
 * Typically, the upper 20 bits of CR3 become the page directory base register (PDBR), 
 * which stores the physical address of the first page directory. 
 *
 * If the PCIDE bit in CR4 is set, the lowest 12 bits are used for the process-context identifier (PCID).[1]
*/
#define __FORCE_ORDER "m"(*(unsigned int *)0x1000UL)

inline unsigned long my_read_cr3(void){
    unsigned long val;
	asm volatile("mov %%cr3,%0\n\t" : "=r" (val) : __FORCE_ORDER);
	return val;
}


static int __init km_init(void){
    unsigned long cr3;
    pr_info("Hello\n");
    cr3 = my_read_cr3();
    pr_info("cr3: 0x%px, PDBR: %px\n", (void *)cr3, (void *)(cr3 & 0xfffffffffffff000));
    pr_info("PDBR: 0x%px\n", (void *)read_cr3_pa());
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


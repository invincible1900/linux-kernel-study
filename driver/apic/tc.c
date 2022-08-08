#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/io.h>

static void tc(void){
        uint32_t value;
        //phys_addr_t apic_base_phys = 0xfee00000; // lapic
        phys_addr_t apic_base_phys = 0xFEC00000; // ioapci
        void __iomem *mapped = ioremap(apic_base_phys, 32);

        if(mapped == NULL){
            printk(KERN_INFO "nullpointer\n");
        } else {
            iowrite32((uint32_t)1, mapped);
            value = ioread32(mapped+16);
            printk(KERN_INFO "Value: 0x%x\n", value); 
        }
        
        iounmap(mapped);
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


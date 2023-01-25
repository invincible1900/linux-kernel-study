#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/version.h>

char *sym = "dump_pagetable";

#if LINUX_VERSION_CODE < KERNEL_VERSION(5,7,0)
int init_kallsyms(void) {
    return 0;
}
#else
typedef unsigned long(*kallsymsFn)(const char *);

static kallsymsFn kallsyms = NULL;

unsigned long kallsyms_lookup_name(const char *name)
{
    return kallsyms(name);
}

int init_kallsyms(void)
{
    struct kprobe kp = {0};
    int ret = 0;
    kp.symbol_name = "kallsyms_lookup_name";

    ret = register_kprobe(&kp);

    if (ret < 0)
        return ret;

    kallsyms = (kallsymsFn)kp.addr;

    unregister_kprobe(&kp);

    return ret;
}
#endif

static void (*dump_pagetable)(unsigned long address);


long run(void){

    dump_pagetable = (void *)kallsyms_lookup_name(sym);
    pr_info("%px, %px\n", __va(read_cr3_pa()), read_cr3_pa());
    //pr_info("%px\n", *(long *)(__va(0x4822000e1)));
    pr_info("dump_pagetable: %px\n", dump_pagetable);
    dump_pagetable((unsigned long)dump_pagetable);
    
    pr_info("sym: %px\n", sym);
    dump_pagetable((unsigned long)sym);

    return 0;

}

static int __init km_init(void){
    init_kallsyms();
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


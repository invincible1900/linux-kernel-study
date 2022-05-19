// Crash 计数器
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/kmsg_dump.h>
#include <linux/fs.h>
#include <linux/vmalloc.h>
#include "counter.h"

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 8, 0)
static const char* kmsg_dump_reason_to_str[] = {
	[KMSG_DUMP_PANIC]	= "panic",
	[KMSG_DUMP_OOPS]	= "oops",
	[KMSG_DUMP_EMERG]	= "emerg",
	[KMSG_DUMP_RESTART]	= "restart",
	[KMSG_DUMP_HALT]	= "halt",
	[KMSG_DUMP_POWEROFF]= "poweroff",
};
#else
static const char* kmsg_dump_reason_to_str[] = {
	[KMSG_DUMP_PANIC]	= "panic",
	[KMSG_DUMP_OOPS]	= "oops",
	[KMSG_DUMP_EMERG]	= "emerg",
	[KMSG_DUMP_SHUTDOWN]	= "shutdown",
};
#endif

static void x_kmsg_dump(struct kmsg_dumper *dumper, enum kmsg_dump_reason reason){
     // 用栈内存
    char path[100] = {0, };

    if(irqs_disabled()) {
		local_irq_enable();
	}

    memset(path, 0, sizeof(path));

    pr_info("[D] Crash reason: %s\n", kmsg_dump_reason_to_str[reason]);

    sprintf(path, "/var/log/x-counter-%s",  kmsg_dump_reason_to_str[reason]);

    file_counter_rcu(path, 1);

    return ;
}

static struct kmsg_dumper x_kmsg_dumper = {
	.dump = x_kmsg_dump,
};

static int __init bugon_t_init(void)
{
    printk(KERN_INFO "Hello \n");

    kmsg_dump_register(&x_kmsg_dumper);

	BUG_ON(1);
    //pr_info("%d\n", *(int *)0);

    return 0;
}

static void __exit bugon_t_exit(void)
{
    //kmsg_dump_unregister(&x_kmsg_dumper);
    printk(KERN_INFO "Goodbye\n");
}

module_init(bugon_t_init);
module_exit(bugon_t_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("0.1");

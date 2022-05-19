#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/kmsg_dump.h>
#include <linux/fs.h>
#include <linux/vmalloc.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
#define kernel_write(a, b, c, d) \
        kernel_write(a, b, c, d)
#else
#define kernel_write(a, b, c, d) \
        kernel_write(a, b, c, &d)
#endif

#define x_file_write x_file_trunc_write // alias
ssize_t x_file_trunc_write(char *path, char *buff, ssize_t buff_sz){
    struct file *filep;
    ssize_t ret;
    loff_t pos = 0;

    filep = filp_open(path, O_WRONLY|O_CREAT|O_TRUNC, 0666);

    if(IS_ERR(filep)){
        pr_err("[D] %s:%d\n", __func__, __LINE__);
        return -1;
    }

    ret = kernel_write(filep, buff, buff_sz, pos);

    filp_close(filep, NULL);

    return ret;
}

static const char* kmsg_dump_reason_to_str[] = {
	[KMSG_DUMP_PANIC]	= "panic",
	[KMSG_DUMP_OOPS]	= "oops",
	[KMSG_DUMP_EMERG]	= "emerg",
	[KMSG_DUMP_RESTART]	= "restart",
	[KMSG_DUMP_HALT]	= "halt",
	[KMSG_DUMP_POWEROFF]= "poweroff",
};

static void x_kmsg_dump(struct kmsg_dumper *dumper, enum kmsg_dump_reason reason){
    char *buff = NULL;
    ssize_t buff_sz = 100;

#if 1
    if(irqs_disabled()) {
		//irq = true;
		local_irq_enable();
	}

#endif
    buff = vmalloc(buff_sz);
    memset(buff, 0, buff_sz);
    pr_info("Hello driver\n");
    sprintf(buff, "Crash reason: %s\n", kmsg_dump_reason_to_str[reason]);
    x_file_write("/var/crash/hellod", buff, buff_sz);
    vfree(buff);
    return ;
}


static void bug_on_t(void){
	BUG_ON(1);
	// WARN_ON(1);
	pr_info("[D] %s-%d\n", __func__, __LINE__);
}


static struct kmsg_dumper x_kmsg_dumper = {
	.dump = x_kmsg_dump,
};

static int __init bug_on_t_init(void)
{
    printk(KERN_INFO "Hello \n");
    kmsg_dump_register(&x_kmsg_dumper);

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

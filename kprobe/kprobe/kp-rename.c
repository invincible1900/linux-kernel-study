#include <linux/init.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/string.h>

static int __kprobes vfs_rename_entry_handler(struct kprobe *p, struct pt_regs *regs)
{
	if (!current->mm)
		return 1;	/* Skip kernel threads */

    pr_info("[+] Hello from vfs_rename\n");

	return 0;
}

static void __kprobes vfs_rename_ret_handler(struct kprobe *p, struct pt_regs *regs,
                unsigned long flags){


    pr_info("[+] Bye from vfs_rename\n");
}

static struct kprobe kp = {
	.symbol_name = "vfs_rename",
    .pre_handler = vfs_rename_entry_handler,
    .post_handler = vfs_rename_ret_handler,
};


int start_vfs_rename_hook(void)
{
	int ret;

	ret = register_kprobe(&kp);
    if (ret < 0) {
        pr_err("register_kprobe failed, returned %d\n", ret);
        return ret;
    }
    pr_info("Planted kprobe at %p\n", kp.addr);

	return 0;
}

void stop_vfs_rename_hook(void)
{
    unregister_kprobe(&kp);
    pr_info("kprobe at %p unregistered\n", kp.addr);
}



int kp_init(void)
{
        start_vfs_rename_hook();
        printk(KERN_INFO "Hello\n");
        return 0;
}

void kp_exit(void)
{
        stop_vfs_rename_hook();
        printk(KERN_INFO "Goodbye\n");
}

module_init(kp_init);
module_exit(kp_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("Kernel Kretprobe Test Module.");
MODULE_VERSION("0.1");

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/string.h>

static int vfs_rename_entry_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	if (!current->mm)
		return 1;	/* Skip kernel threads */

    pr_info("[+] Hello from vfs_rename\n");

	return 0;
}


static int vfs_rename_ret_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{

    pr_info("[+] Bye from vfs_rename\n");
	return 0;
}

static struct kretprobe vfs_rename_kretprobe = {
	.kp.symbol_name		= "vfs_rename",
	.handler		= vfs_rename_ret_handler,
	.entry_handler		= vfs_rename_entry_handler,
	/* Probe up to 20 instances concurrently. */
	.maxactive		= 20,
};

int start_vfs_rename_hook(void)
{
	int ret;

	ret = register_kretprobe(&vfs_rename_kretprobe);
	if (ret < 0) {
		pr_err("register_kretprobe %s failed, returned %d\n",vfs_rename_kretprobe.kp.symbol_name,  ret);
		return -1;
	}
	pr_info("Planted return probe at %s: %p\n",
			vfs_rename_kretprobe.kp.symbol_name, vfs_rename_kretprobe.kp.addr);
	return 0;
}

void stop_vfs_rename_hook(void)
{
	unregister_kretprobe(&vfs_rename_kretprobe);
	pr_info("kretprobe at %p unregistered\n", vfs_rename_kretprobe.kp.addr);

	/* nmissed > 0 suggests that maxactive was set too low. */
	pr_info("Missed probing %d instances of %s\n",
		vfs_rename_kretprobe.nmissed, vfs_rename_kretprobe.kp.symbol_name);
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

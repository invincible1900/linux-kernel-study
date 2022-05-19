// 关机前写一个文件, But won't work!

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/vmalloc.h>
#include <linux/reboot.h>

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


static int rw_ufile(struct notifier_block *notifier, unsigned long val,
              void *v){
    char *buff = NULL;
    ssize_t buff_sz = 100;

    buff = vmalloc(buff_sz);
    pr_info("Hello from lkm\n");
    sprintf(buff, "Hello from lkm\n");
    x_file_write("/root/hellod", buff, buff_sz);
    vfree(buff);
    return 0;
}

static struct notifier_block x_reboot_notifier = {
    .notifier_call = rw_ufile,
    .priority = 0,
};

static int __init rw_ufile_init(void)
{
    printk(KERN_INFO "Hello\n");
    register_reboot_notifier(&x_reboot_notifier);
    return 0;
}

static void __exit rw_ufile_exit(void)
{
    printk(KERN_INFO "Goodbye\n");
}


module_init(rw_ufile_init);
module_exit(rw_ufile_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("0.1");


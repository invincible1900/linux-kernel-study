#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/vmalloc.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
#define kernel_write(a, b, c, d) \
        kernel_write(a, b, c, d)
#else
#define kernel_write(a, b, c, d) \
        kernel_write(a, b, c, &d)
#endif

static void rw_ufile(void){
    struct file *filep;
    char *buff;
    loff_t pos = 0;
    size_t ret = 0;

    buff = vmalloc(100);
    sprintf(buff, "%s, %d\n", __func__, __LINE__);

    /* Create and write  */
    filep = filp_open("/root/hello", O_WRONLY|O_CREAT|O_TRUNC, 0666);
    kernel_write(filep, buff, strlen(buff), pos);
    filp_close(filep, NULL);

    memset(buff, 0, 100);

    /* read */
    filep = filp_open("/root/hello", O_RDWR, 0);
    ret = kernel_read(filep, buff, 100, 0);
    pr_info("%ld\n%s\n", ret, buff);

    vfree(buff);
}


static int __init rw_ufile_init(void)
{
    printk(KERN_INFO "Hello\n");
    rw_ufile();
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


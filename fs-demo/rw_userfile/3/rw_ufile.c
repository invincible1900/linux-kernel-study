// 一个基于磁盘文件的计数器

#include "rw_ufile.h"

static long file_counter_rcu(long val){
    char *path = PATH;

    char *buff = NULL;
    ssize_t buff_sz = 100;

    ssize_t ret = 0;
    long rw_val = -1;

    /* [] Alloc buff  */
    buff = vmalloc(buff_sz);
    if(!buff){
        pr_err("[D] %s:%d\n", __func__, __LINE__);
        goto err;
    }
    /* [] Alloc buff end */

    /* [] File exists ? pass: create */
    ret = x_file_exists(path);
    if(ret < 0){
            sprintf(buff, "%ld", 0UL);
            ret = x_file_write(path, buff, buff_sz);
            if(ret < 0){
                    goto err;
            }
    }
    /* [] File exists? end */

    /* [] File read */
    ret = x_file_read(path, buff, buff_sz);
    if(ret < 0){
        goto err;
    }
    if(kstrtol(buff, 10, &rw_val) < 0){
        goto err;
    }
    /* [] File read end*/


    /* [] File modify and write back */
    rw_val += val;

    memset(buff, 0, buff_sz);
    sprintf(buff, "%ld", rw_val);

    ret = x_file_write(path, buff, buff_sz);
    if(ret < 0){
        goto err;
    }
    /* [] File modify and write back end*/

err:
    if(buff){
        vfree(buff);
    }

    return rw_val;
}


static int __init rw_ufile_init(void)
{
    long ret;
    printk(KERN_INFO "Hello\n");
    ret = file_counter_rcu(1);
    pr_info("counter: %ld\n", ret);
    return 0;
}

static void __exit rw_ufile_exit(void)
{
    long ret;
    ret = file_counter_rcu(-1);
    pr_info("counter: %ld\n", ret);
    printk(KERN_INFO "Goodbye\n");
}


module_init(rw_ufile_init);
module_exit(rw_ufile_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("0.1");


/*
 * Dome for:  
 *	- sysfs dir create
 *	- sysfs file create
 *	- sysfs file delete
 *	- sysfs file read
 *	- sysfs fiel write
 */

#include <linux/module.h>
#include <linux/kernel.h>

#define MSG_MAX 4096

#define DEBUG(fmt, ...) pr_info("[D] %s:%d, "fmt, __func__, __LINE__, ##__VA_ARGS__)

static ssize_t xx_read(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
	ssize_t count;
	count = sprintf(buf, "%s\n", "nothing");
	DEBUG("count: %lu\n", count);
	return count;
	
}

ssize_t xx_write(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count){

	DEBUG("count: %lu\n", count);

	if(!count || count > MSG_MAX){
		DEBUG("error\n");
		return -1;
	}
	pr_info("%s\n", buf);	
	
	return count;

}

/* 创建目录 */
struct kobject *xx_root = NULL; 

/* 创建文件 */
static struct kobj_attribute xx_file = __ATTR(xx, 0660, xx_read, xx_write);
static struct kobj_attribute xx_file2 = __ATTR(xx2, 0660, xx_read, xx_write);

static int sysfs_t(void){
	int ret;

	/* 创建目录 */
	xx_root = kobject_create_and_add("xx_root", NULL);
	if(!xx_root){
		DEBUG("error\n");
		ret = -1;
		goto out;

	}

	/* 创建文件 */
        ret = sysfs_create_file(xx_root, &xx_file.attr);
	if(ret != 0){
		DEBUG("error\n");
		goto err1;
	}

        ret = sysfs_create_file(xx_root, &xx_file2.attr);
	if(ret != 0){
		DEBUG("error\n");
		goto err2;
	}
	goto out;

err2:
        sysfs_remove_file(xx_root, &xx_file.attr);
err1:
	kobject_put(xx_root);
	xx_root = NULL;
out:
	return ret;
}


static int __init sysfs_t_init(void)
{
	DEBUG("Hello!\n");
	sysfs_t();
	return 0;
}

static void __exit sysfs_t_exit(void)
{
	if(xx_root){
		/* 删除文件 */
       		sysfs_remove_file(xx_root, &xx_file2.attr);
        	sysfs_remove_file(xx_root, &xx_file.attr);

		/* 删除目录 */
		kobject_put(xx_root);
	}
}


module_init(sysfs_t_init);
module_exit(sysfs_t_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("0.1");

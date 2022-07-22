#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/rculist.h>
#include <linux/slab.h>

#define MSG_MAX 4096

#define DEBUG(fmt, ...) pr_info("[D] %s:%d, "fmt, __func__, __LINE__, ##__VA_ARGS__)

struct x_msg {
    char msg[100];
    struct list_head list;
};

LIST_HEAD(head);

void clear_list(void){
    struct x_msg *msg;

    // TODO: spin_lock
    list_for_each_entry_rcu(msg, &head, list){
            list_del_rcu(&(msg->list));
            synchronize_rcu();
            kfree(msg);
    }
}

static ssize_t xx_read(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
	ssize_t count = 0;

    struct x_msg *msg;

    rcu_read_lock();
    list_for_each_entry_rcu(msg, &head, list){
	    count += sprintf(buf+count, "%s\n", msg->msg);
    }
    rcu_read_unlock();

	DEBUG("count: %lu\n", count);

	return count;
	
}

ssize_t xx_write(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count){
    struct x_msg *msg;

	DEBUG("count: %lu\n", count);

	if(!count || count > MSG_MAX){
		DEBUG("error\n");
		return -1;
	}
	pr_info("%s\n", buf);	

    msg = kmalloc(sizeof(struct x_msg), GFP_KERNEL);
    if(!msg){
		DEBUG("error\n");
        return  -1;
    }
    
    if(strcmp(buf, "xxx") == 0 || strcmp(buf, "xxx\n") == 0){
            clear_list();
            return count;
    }

    memcpy(msg->msg, buf, 100);

    // TODO: spin_lock
    list_add_rcu(&(msg->list), &head);
	
	return count;

}

/* 创建目录 */
struct kobject *xx_root = NULL; 

/* 创建文件 */
static struct kobj_attribute xx_file = __ATTR(xx, 0660, xx_read, xx_write);

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

	goto out;

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

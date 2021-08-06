/*
 * 测试数据读写
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/vmalloc.h>

#define MSG_MAX 1024

#define DEBUG(fmt, ...) pr_info("[D] %s:%d, "fmt, __func__, __LINE__, ##__VA_ARGS__)

struct list_head msg_list;

struct msg_item {
	char *msg;
	size_t len;
	struct list_head node;
};

static int push_msg(const char *buf, size_t len){
	int ret = 0;
	char *tmp = NULL;
	struct msg_item *msg = NULL;

	/* strip '\0'  */
	while(buf[len-1] == '\0'){
		len -= 1;
		if(len == 0){
			return 0;
		}
	}

	msg = vmalloc(sizeof(struct msg_item));
	memset(msg, 0, sizeof(struct msg_item)); 

	if(!msg){
		DEBUG("error\n");
		ret = -1;
		goto err;
	}

	tmp = vmalloc(len + 1);	 // +1 for '\0'
	memset(tmp, 0, len);

	if(!tmp){
		DEBUG("error\n");
		ret = -1;
		goto err;
	}

	memcpy(tmp, buf, len);
	msg->msg = tmp;
	msg->len = len;
	DEBUG("%s\n", msg->msg);
	list_add_tail(&msg->node, &msg_list);

	goto out;	
err:
	if(tmp){
		vfree(tmp);
	}

	if(msg){
		vfree(msg);
	}	
out:
	return ret;

}

static ssize_t xx_read(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
	struct msg_item *msg, *tmp;
	ssize_t count = 0;
	char *pos = buf;

	list_for_each_entry_safe(msg, tmp, &msg_list, node){
		if(count + msg->len + 1 > 4096){
			DEBUG("msg too large\n");
			return count;
		}
		DEBUG("pos: %px, len: %ld, msg: %s\n", pos, msg->len, msg->msg);
		memcpy(pos, msg->msg, msg->len);
		pos[msg->len] = '\n';
		count += msg->len + 1;
		pos += msg->len + 1;
		DEBUG("pos: %px, count: %ld\n\n",  pos, count);
	

		
	}
	return count;
	
}

ssize_t xx_write(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count){

	// DEBUG("count: %lu\n", count);

	if(!count || count > MSG_MAX){
		DEBUG("error\n");
		return -1;
	}

	if(push_msg(buf, count) < 0){
		DEBUG("error\n");
		return -1;
	}
	
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
	INIT_LIST_HEAD(&msg_list);
	sysfs_t();
	return 0;
}

static void __exit sysfs_t_exit(void)
{
	struct msg_item *msg, *tmp;
	list_for_each_entry_safe(msg, tmp, &msg_list, node){
		list_del(&msg->node);
		vfree(msg->msg);
		vfree(msg);
	}

	
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

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kallsyms.h>
#include <linux/sched/signal.h>
#include <linux/uaccess.h>

static void tc(void);

static ssize_t xx_read(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
	ssize_t count;
    count = sprintf(buf, "try demsg\n");
    tc();
	return count;
}

ssize_t xx_write(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count){
	return count;
}

struct kobject *xx_root = NULL; 
static struct kobj_attribute xx_file = __ATTR(xx, 0660, xx_read, xx_write);

static int sysfs_t(void){
	int ret;
	xx_root = kobject_create_and_add("xx_root", NULL);
    ret = sysfs_create_file(xx_root, &xx_file.attr);

	return ret;
}


static inline void *task_stack_page(const struct task_struct *task)
{
	return task->stack;
}

/*
#define task_pt_regs(task) \
({									\
	unsigned long __ptr = (unsigned long)task_stack_page(task);	\
	__ptr += THREAD_SIZE - TOP_OF_KERNEL_STACK_PADDING;		\
	((struct pt_regs *)__ptr) - 1;					\
})
*/

static void tc(void){
        struct pt_regs *regs;
        unsigned long sp, ip;

        pr_info("%s, %d\n", current->comm, current->pid);

        regs = task_pt_regs(current);

        if(copy_from_user((void *)&sp, (void *)regs->sp, 8)){
                pr_info("err\n");
        }

        if(copy_from_user((void *)&ip, (void *)regs->ip, 8)){
                pr_info("err2\n");
        }
        pr_info("sp: %px, %px\n", (void *)regs->sp, (void *)sp);
        pr_info("ip: %px, %px\n", (void *)regs->ip, (void *)ip);
                
}


static int __init tc_init(void)
{
    pr_info("Hello tc\n");

    sysfs_t();

    return 0;
}

static void __exit tc_exit(void)
{   
    sysfs_remove_file(xx_root, &xx_file.attr);
    kobject_put(xx_root);

    pr_info("Goodbye tc\n");
}


module_init(tc_init);
module_exit(tc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("1.0");


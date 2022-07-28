#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kallsyms.h>
#include <linux/sched/signal.h>
#include <linux/uaccess.h>

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
        struct task_struct *g, *t;
        struct pt_regs *regs;

        do_each_thread(g, t){
            if(strcmp("a.out", t->comm) != 0) continue;

            pr_info("%s, %d\n", t->comm, t->pid);
            regs = task_pt_regs(t);
            pr_info("sp: %px\n", (void *)regs->sp);
            pr_info("ip: %px\n", (void *)regs->ip);
                
        } while_each_thread(g, t);
}


static int __init tc_init(void)
{
    pr_info("Hello tc\n");

#ifdef CONFIG_THREAD_INFO_IN_TASK
        pr_info("CONFIG_THREAD_INFO_IN_TASK=y");
#else
        pr_info("CONFIG_THREAD_INFO_IN_TASK=n");
#endif

    tc();
    return 0;
}

static void __exit tc_exit(void)
{
    pr_info("Goodbye tc\n");
}


module_init(tc_init);
module_exit(tc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("1.0");


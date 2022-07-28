#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/stacktrace.h>
#include <asm/stacktrace.h>
#include <linux/kallsyms.h>

unsigned int (* x_stack_trace_save_tsk)(struct task_struct *task,
                  unsigned long *store, unsigned int size,
                  unsigned int skipnr);


#define MAX_STACK_TRACE_DEPTH   64

static unsigned long stack_entries[MAX_STACK_TRACE_DEPTH];

static void tc(void){
        int i = 0;
        struct task_struct *g, *t;

        x_stack_trace_save_tsk = (void *)kallsyms_lookup_name("stack_trace_save_tsk");

        do_each_thread(g, t) {
        
            x_stack_trace_save_tsk(t, 
                        (void *)&stack_entries, MAX_STACK_TRACE_DEPTH, 0);

             for(i = 0; i<MAX_STACK_TRACE_DEPTH; i++){
                    if(stack_entries[i])
                        pr_info("[%s] %d, %px, %pB\n", t->comm, t->pid,
                           (void *)stack_entries[i], (void *)stack_entries[i]);
             }       
        
        } while_each_thread(g, t);
        return;
}


static int __init tc_init(void)
{
    //pr_info("Hello tc\n");
    tc();
    return 0;
}

static void __exit tc_exit(void)
{
    //pr_info("Goodbye tc\n");
}


module_init(tc_init);
module_exit(tc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("1.0");


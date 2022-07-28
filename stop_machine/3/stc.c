#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stop_machine.h>
#include <linux/kallsyms.h>
#include <linux/sched/signal.h>
#include <linux/delay.h>

// fs/proc/base.c 
#define MAX_STACK_TRACE_DEPTH 64

// kernel/stacktrace.c
unsigned int (*x_stack_trace_save_tsk)(struct task_struct *task,
        unsigned long *store, unsigned int size,
        unsigned int skipnr);


unsigned long stack_entries[MAX_STACK_TRACE_DEPTH];

int tc(void *data){
        int nr_entries, i, cpu;
        struct task_struct *g, *t;
        cpu = get_cpu();
        pr_info("[D] get_cpu: %d\n", cpu);

        x_stack_trace_save_tsk = 
                (void *)kallsyms_lookup_name("stack_trace_save_tsk");

        if(!x_stack_trace_save_tsk){
                pr_err("[D] stack_trace_save_tsk not found\n");
        }

        do_each_thread(g, t) {
            nr_entries = x_stack_trace_save_tsk(t,
                     (void *)stack_entries, MAX_STACK_TRACE_DEPTH, 0);
            if(strcmp(t->comm, "a.out") == 0){
                pr_info("[D] %d, %s, %d, %d\n", 
                                cpu, t->comm, t->pid, t->recent_used_cpu);
                for (i = 0; i < nr_entries; i++) {
                    if (stack_entries[i] == ULONG_MAX)
                        break;

                    pr_info("\t%px, %pB\n", 
                                    (void *)stack_entries[i], (void *)stack_entries[i]);
                }
            }
        } while_each_thread(g, t);
        return 0;
}


static int __init tc_init(void)
{
    pr_info("Hello tc\n");
    //stop_machine(tc, NULL, cpu_online_mask);
    //stop_machine(tc, NULL, NULL);
    stop_machine(tc, NULL, cpumask_of(1));
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


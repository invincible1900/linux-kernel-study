#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stop_machine.h>

int tc(void *data){
        pr_info("in_interrupt: %ld, in_softirq: %ld, irqs_disabled: %d\n", 
                        in_interrupt(), in_softirq(), irqs_disabled());
        pr_info("cpu: %d, %s\n", get_cpu(), (char *)data);
        return 0;
}

static int __init tc_init(void)
{
    char *s;
    pr_info("Hello tc\n");

    s = "stop_machine(.., .., NULL)";
    stop_machine(tc, s, NULL);

    s = "stop_machine(.., .., cpumask_of(1))";
    stop_machine(tc, s, cpumask_of(1));

    s = "stop_machine(.., .., cpu_online_mask)";
    stop_machine(tc, s, cpu_online_mask);
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

/*********************************************************************
[  640.433594] Hello tc
[  640.434838] in_interrupt: 0, in_softirq: 0, irqs_disabled: 1
[  640.438558] cpu: 0, stop_machine(.., .., NULL)
[  640.442265] in_interrupt: 0, in_softirq: 0, irqs_disabled: 1
[  640.445268] cpu: 1, stop_machine(.., .., cpumask_of(1))
[  640.448451] in_interrupt: 0, in_softirq: 0, irqs_disabled: 1
[  640.448457] in_interrupt: 0, in_softirq: 0, irqs_disabled: 1
[  640.448460] in_interrupt: 0, in_softirq: 0, irqs_disabled: 1
[  640.448463] in_interrupt: 0, in_softirq: 0, irqs_disabled: 1
[  640.448465] cpu: 1, stop_machine(.., .., cpu_online_mask)
[  640.448467] cpu: 2, stop_machine(.., .., cpu_online_mask)
[  640.451986] cpu: 0, stop_machine(.., .., cpu_online_mask)
[  640.455895] cpu: 3, stop_machine(.., .., cpu_online_mask)
[  640.469907] Goodbye tc

typedef struct cpumask 
	unsigned long bits[(((NR_CPUS) + (8) - 1) / (8))]
} cpumask_t;

(gdb) pt cpumask_t
type = struct cpumask {
    unsigned long bits[1];
}

17 typedef struct cpumask { DECLARE_BITMAP(bits, NR_CPUS); } cpumask_t;


10 #define DECLARE_BITMAP(name,bits) \
11     unsigned long name[BITS_TO_LONGS(bits)]

// include/linux/bitops.h
#define BITS_TO_LONGS(nr) DIV_ROUND_UP(nr, BITS_PER_TYPE(long ))

// include/linux/kernel.h
93 #define DIV_ROUND_UP __KERNEL_DIV_ROUND_UP
	// include/uapi/linux/kernel.h
	13 #define __KERNEL_DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))

14 #define BITS_PER_TYPE(type) (sizeof(type) * BITS_PER_BYTE)

#define BITS_PER_BYTE 8
*********************************************************************/

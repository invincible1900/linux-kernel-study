#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/hrtimer.h>

static struct hrtimer my_timer;

static enum hrtimer_restart tick(struct hrtimer *timer){
    pr_info("tick, from: %s\n", current->comm);
    return HRTIMER_NORESTART;
}

static void tc(void){
    struct timespec64 ts = {.tv_sec = 5};
    
    hrtimer_init(&my_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    my_timer.function = tick;
    hrtimer_start(&my_timer, timespec64_to_ktime(ts), HRTIMER_MODE_REL);
    pr_info("hrtimer started, ...\n");

    return;
}


static int __init tc_init(void)
{
    pr_info("Hello tc\n");
    tc();
    return 0;
}

static void __exit tc_exit(void)
{
    // hrtimer_try_to_cancel(&my_timer);
    hrtimer_cancel(&my_timer);
    pr_info("Goodbye tc\n");
}


module_init(tc_init);
module_exit(tc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("1.0");


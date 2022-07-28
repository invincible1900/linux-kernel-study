/*
 * 不在指定CPU上运行的进程也会停止
 * */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stop_machine.h>
#include <linux/delay.h>

unsigned long start;

int tc(void *data){
#if 0
        msleep(10000);
#else
        int cpu, i;
        cpu = get_cpu();
        for(i=0;i<4000;i++){pr_info("[cpu %d]: %d\n", cpu, i);}
        //while(1);
#endif
        return 0;
}


static int __init tc_init(void)
{
    pr_info("Hello tc\n");
    start = jiffies;
    stop_machine(tc, NULL, cpumask_of(1));
    return 0;
}

static void __exit tc_exit(void)
{
    pr_info("%ld\n", jiffies - start);
    pr_info("Goodbye tc\n");
}


module_init(tc_init);
module_exit(tc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("1.0");
/*
 * This process will stop after insmod
#include <stdio.h>

int main(){
        int i = 0;
        while(1){
                i ++;
                if(i%1000000 == 0)
                        printf("%d\n", i);
        }
} 
*/ 

/* msleep crash log:
[ 1489.102291] Hello tc
[ 1489.102791] BUG: scheduling while atomic: migration/0/11/0x00000001
[ 1489.103538] Modules linked in: tc(O+) [last unloaded: tc]
[ 1489.104243] CPU: 0 PID: 11 Comm: migration/0 Tainted: G           O      5.4.0 #1
[ 1489.105200] Hardware name: QEMU Standard PC (i440FX + PIIX, 1996), BIOS 1.13.0-1ub4
[ 1489.106302] Call Trace:
[ 1489.106665]  dump_stack+0x50/0x70
[ 1489.107122]  __schedule_bug.cold+0x47/0x53
[ 1489.107666]  __schedule+0x513/0x5a0
[ 1489.108097]  ? __switch_to_asm+0x34/0x70
[ 1489.108610]  ? __switch_to_asm+0x40/0x70
[ 1489.109133]  ? __switch_to_asm+0x40/0x70
[ 1489.109646]  schedule+0x34/0xa0
[ 1489.110030]  schedule_timeout+0x150/0x2f0
[ 1489.110552]  ? __switch_to_asm+0x40/0x70
[ 1489.111112]  ? __next_timer_interrupt+0xc0/0xc0
[ 1489.111699]  msleep+0x25/0x30
[ 1489.112078]  tc+0xa/0xd [tc]
[ 1489.112464]  multi_cpu_stop+0x5b/0xe0
[ 1489.112955]  ? stop_machine_yield+0x10/0x10
[ 1489.113513]  cpu_stopper_thread+0x63/0xe0
[ 1489.114001]  smpboot_thread_fn+0xc0/0x160
[ 1489.114531]  kthread+0xf6/0x130
[ 1489.114952]  ? sort_range+0x20/0x20
[ 1489.115426]  ? kthread_park+0x80/0x80
[ 1489.115860]  ret_from_fork+0x35/0x40
 * */

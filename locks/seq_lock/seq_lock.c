#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/seqlock.h>

//seqlock_t mr_seq_lock = 

DEFINE_SEQLOCK(mr_seq_lock);

int a = 0;

static void test_mod(void){
        int seq;

        write_seqlock(&mr_seq_lock);
        /* write lock is obtained... */
        a = 1;
        write_sequnlock(&mr_seq_lock);

        do {
            seq = read_seqbegin(&mr_seq_lock);
            /* read data here ... */
            pr_info("[D] %d\n", a);
        } while (read_seqretry(&mr_seq_lock, seq));

}


static int __init test_mod_init(void)
{
    printk(KERN_INFO "Hello test_mod\n");
    test_mod();
    return 0;
}

static void __exit test_mod_exit(void)
{
    printk(KERN_INFO "Goodbye test_mod\n");
}


module_init(test_mod_init);
module_exit(test_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("0.1");


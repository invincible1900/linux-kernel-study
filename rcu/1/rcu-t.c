#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>

struct foo {
  int a;
  int b;
  int c;
};
struct foo *gp = NULL;

void do_something_with(int a, int b, int c){
    a = b + c;
    return;
}

void rcu_t(void){
    struct foo *p = NULL;

    p = kmalloc(sizeof(*p), GFP_KERNEL);
    p->a = 1;
    p->b = 2;
    p->c = 3;
    gp = p;
    // pr_info("%px, %d, %d, %d, %px\n", gp, gp->a, gp->b, gp->c, p);
    // rcu_assign_pointer(gp, p);
#if 0
    p = gp;
    if (p != NULL) {
      do_something_with(p->a, p->b, p->c);
    }
#endif
}

void rcu_t2(void){
    struct foo *p = NULL;

    p = kmalloc(sizeof(*p), GFP_KERNEL);
    p->a = 1;
    p->b = 2;
    p->c = 3;
    rcu_assign_pointer(gp, p);
    //pr_info("%px, %d, %d, %d, %px\n", gp, gp->a, gp->b, gp->c, p);
#if 0
    rcu_read_lock();
    p = rcu_dereference(gp);
    if (p != NULL) {
      do_something_with(p->a, p->b, p->c);
    }
    rcu_read_unlock();
#endif
}


static int __init rcu_t_init(void)
{
    printk(KERN_INFO "Hello rcu_t\n");
    rcu_t();
    rcu_t2();
    return 0;
}

static void __exit rcu_t_exit(void)
{
    printk(KERN_INFO "Goodbye rcu_t\n");
}


module_init(rcu_t_init);
module_exit(rcu_t_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("0.1");


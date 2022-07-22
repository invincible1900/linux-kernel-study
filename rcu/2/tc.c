#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/rculist.h>

struct foo {
    struct list_head list;
    int a;
    int b;
    int c;
};
LIST_HEAD(foo_head);


void print_list(void){
    struct foo *fp;
    
    list_for_each_entry_rcu(fp, &foo_head, list){
            pr_info("%d, %px\n", fp->a, fp);
            list_del_rcu(&(fp->list));
    }    
}

void rcu_t(void){
    struct foo *p = NULL;
    struct foo *q = NULL;
    pr_info("%px, %px, %px\n", &foo_head, foo_head.next, foo_head.prev);
    p = kmalloc(sizeof(*p), GFP_KERNEL);
    p->a = 1;
    p->b = 2;
    p->c = 3;
    list_add(&(p->list), &foo_head);
    pr_info("%px, %px, %px\n", &foo_head, foo_head.next, foo_head.prev);

    q = kmalloc(sizeof(*p), GFP_KERNEL);
    q->a = 4;
    q->b = 5;
    q->c = 6;
    list_add_rcu(&(q->list), &foo_head);
    pr_info("%px, %px, %px\n", &foo_head, foo_head.next, foo_head.prev);
    if(*(int *)0xffffffffa0000000 !=0 ){
    print_list();}
}


static int __init rcu_t_init(void)
{
    int i = 0;
    printk(KERN_INFO "Hello rcu_t\n");
    for( i = 0; i < 10; i++){
        rcu_t();
    }
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


/*
 * Slab uaf test.(test this in a vm)
 *
 * */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/slub_def.h>
#include <linux/slab.h>
#include <linux/sched.h>

#define OBJ_SIZE 4097
#define OBJ_NUM 10

struct my_struct {
    union {
        char data[OBJ_SIZE];
        struct {
            void (*func)(void);
            char paddings[OBJ_SIZE - 8];
        };
    };
};

static struct kmem_cache *my_cachep;
static struct my_struct *ms[OBJ_NUM];

void hello_func(void){
    pr_info("Hello\n");
}

void hack_func(void){
    pr_info("Hacked\n");
}

static int __init km_init(void){
    int i;
    struct my_struct *m;

    my_cachep = kmem_cache_create("my_struct", 
            sizeof(struct my_struct), 0,
            SLAB_HWCACHE_ALIGN | SLAB_PANIC | SLAB_ACCOUNT,
            NULL);


    for(i = 0; i<OBJ_NUM; i++){
        ms[i] = kmem_cache_alloc(my_cachep, GFP_KERNEL);
        ms[i]->func = (void  *)hello_func;
    }

    // UAF
    ms[0]->func();
    pr_info("free ms[0]: %px\n", ms[0]);
    kmem_cache_free(my_cachep, ms[0]);

    // other task
    for(i = 0; i<100; i++){
        m = kmem_cache_alloc(my_cachep, GFP_KERNEL);
        if(m == ms[0]){
            pr_info("[%d] Successfully reallocated: %px\n", i, m);
            m->func = (void *)hack_func;
            break;
        }
    }
    // may crash
    ms[0]->func();

    return 0;

}

static void __exit km_exit(void)
{
    int i;

    for( i = 0; i<OBJ_NUM; i++){
        kmem_cache_free(my_cachep, ms[i]);
    }
    kmem_cache_destroy(my_cachep);
    pr_info("Bye\n");
}


module_init(km_init);
module_exit(km_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("0.1");


/*
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
    char data[OBJ_SIZE];
};

static struct kmem_cache *my_cachep;
static struct my_struct *ms[OBJ_NUM];

static int __init km_init(void){
    int i, cpu;
    struct kmem_cache_cpu *c;

    my_cachep = kmem_cache_create("my_struct", 
            sizeof(struct my_struct), 0,
            SLAB_HWCACHE_ALIGN | SLAB_PANIC | SLAB_ACCOUNT,
            NULL);

    pr_info("my_cachep: %px\n", my_cachep);
    pr_info("my_cachep.size: %u\n", my_cachep->size);
    pr_info("my_cachep.object_size: %u\n", kmem_cache_size(my_cachep));

    cpu = get_cpu();
    c = per_cpu_ptr(my_cachep->cpu_slab, cpu);
    pr_info("cpu: %d, %px\n", cpu, c);
    pr_info("freelist: %px\n", c->freelist);
    pr_info("page: %px\n", c->page);

    for(i = 0; i<OBJ_NUM; i++){
        pr_info("ba: freelist: %px, page: %px\n", c->freelist, c->page);
        ms[i] = kmem_cache_alloc(my_cachep, GFP_KERNEL);
        pr_info("cache alloc: %px\n", ms[i]);
        pr_info("aa: freelist: %px, page: %px\n\n", c->freelist, c->page);
    }

    //pr_info("freelist: %px\n", c->freelist);
    //pr_info("page: %px\n", c->page);
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


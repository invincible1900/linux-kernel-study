/*
 * 查看 SLUB struct kmem_cache_cpu 各个字段
 *
 * struct kmem_cache {
 *   struct kmem_cache_cpu __percpu *cpu_slab;
 * }
 *
 * struct kmem_cache_cpu {
 *      void **freelist;
 *      struct page *page;
 *      struct page *partial;
 * }
 *
 * */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/slub_def.h>
#include <linux/sched.h>

#define OBJ_SIZE (64)
#define OBJ_NUM (PAGE_SIZE/OBJ_SIZE + 1)

struct my_struct {
    char data[OBJ_SIZE];
};

static struct kmem_cache *my_cachep;
static struct my_struct *ms[OBJ_NUM];

static int __init km_init(void){
    int i, cpu;
    struct kmem_cache_cpu *cs;

    cpu = get_cpu();

    my_cachep = kmem_cache_create("my_struct", 
            sizeof(struct my_struct), 0,
            SLAB_HWCACHE_ALIGN | SLAB_PANIC | SLAB_ACCOUNT,
            NULL);

    pr_info("kmem_cache: %px\n", my_cachep);
    pr_info("kmem_cache.size: %u\n", my_cachep->size);
    pr_info("kmem_cache.object_size: %u\n", my_cachep->object_size);

    cs = per_cpu_ptr(my_cachep->cpu_slab, cpu);
    pr_info("cpu: %d, cpu_slab: %px\n", cpu, cs);
    pr_info("cpu_slab.freelist: %px\n", cs->freelist);
    pr_info("cpu_slab.page: %px, page_addr: %px\n", cs->page, page_address(cs->page));

    for(i = 0; i<OBJ_NUM; i++){
        pr_info("ba: freelist: %px, page_addr: %px, %d, partial page: %px(%px), %d\n", 
                cs->freelist, page_address(cs->page), cs->page->pages,
                cs->partial, page_address(cs->partial),
                cs->partial->pages
                
                );

        ms[i] = kmem_cache_alloc(my_cachep, GFP_KERNEL);
        pr_info("cache alloc:  %px\n", ms[i]);
        //pr_info("aa: freelist: %px, page_addr: %px\n\n", cs->freelist, page_address(cs->page));
    }
    pr_info("kmem_cache.offset: %u\n", my_cachep->offset);

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


/*
 * 查看 slub 分配 object 和 page 的关系
 *
 * 内核编译选项需要 unset CONFIG_SLAB_MERGE_DEFAULT
 *
 * */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/slub_def.h>
#include <linux/sched.h>

#define OBJ_SIZE 256
#define OBJ_NUM ((PAGE_SIZE/OBJ_SIZE) * 3)

struct my_struct {
    char data[OBJ_SIZE];
};

static struct kmem_cache *my_cachep;
static struct my_struct *ms[OBJ_NUM];

static int __init km_init(void){
    int i, cpu;
    struct kmem_cache_cpu *c;
    struct page *pg;
#define OO_SHIFT    16
#define OO_MASK     ((1 << OO_SHIFT) - 1)
    pr_info("Hello\n");

    my_cachep = kmem_cache_create("my_struct", 
            sizeof(struct my_struct), 0,
            SLAB_HWCACHE_ALIGN | SLAB_PANIC | SLAB_ACCOUNT,
            NULL);

    pr_info("my_cachep: %px, %s\n", my_cachep, my_cachep->name);
    pr_info("my_cachep.size: %u\n", my_cachep->size);
    pr_info("my_cachep.object_size: %u\n", kmem_cache_size(my_cachep));
    pr_info("my_cachep.cpu_partial: %d\n", my_cachep->cpu_partial);
    pr_info("my_cachep.objs_per_cpu: %d\n", my_cachep->oo.x & OO_MASK);

    cpu = get_cpu();
    pr_info("cpu: %d\n", cpu);

    c = per_cpu_ptr(my_cachep->cpu_slab, cpu);

    for(i = 0; i<OBJ_NUM; i++){
        ms[i] = kmem_cache_alloc(my_cachep, GFP_KERNEL);
        pg = virt_to_page(ms[i]);
        pr_info("[%02d] object: %px, page: %px(%px), %d\n", i, ms[i], 
                pg, page_address(pg),
                (void *)pg == (void *)c->page);
    }

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


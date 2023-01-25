/*
 * 遍历 kmem_cache 的 list
 *
 * LIST_HEAD(slab_caches);
 *
 * struct kmem_cache {
 *   struct kmem_cache_cpu __percpu *cpu_slab;
 *   unsigned int size;
 *   unsigned int object_size;
 *   unsigned int offset
 *   struct list_head list;
 *   struct kmem_cache_node *node[MAX_NUMNODES]
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
#include <linux/kprobes.h>

#define OBJ_SIZE 128
//#define OBJ_NUM (PAGE_SIZE/OBJ_SIZE)
#define OBJ_NUM 10 

struct my_struct {
    char data[OBJ_SIZE];
};

static struct kmem_cache *my_cachep;
static struct my_struct *ms[OBJ_NUM];


static int __init km_init(void){
    int i;
    struct kmem_cache *cp;

    my_cachep = kmem_cache_create("my_struct", 
            sizeof(struct my_struct), 0,
            SLAB_HWCACHE_ALIGN | SLAB_PANIC | SLAB_ACCOUNT,
            NULL);

    for(i = 0; i<OBJ_NUM; i++){
        ms[i] = kmem_cache_zalloc(my_cachep, GFP_KERNEL);
    }

    pr_info("0x%px: %s, object_size: %d\n", my_cachep, my_cachep->name, my_cachep->object_size);
    list_for_each_entry(cp, &my_cachep->list, list){
        pr_info("0x%px: %s\n", cp, cp->name);
    }    

    return 0;

}

static void __exit km_exit(void)
{
    int i;
    for(i = 0; i<OBJ_NUM; i++){
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


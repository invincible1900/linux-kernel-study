#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
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
    int i;

    my_cachep = kmem_cache_create("my_struct", sizeof(struct my_struct), 
                        ARCH_MIN_TASKALIGN, SLAB_PANIC , NULL);

    pr_info("my_cachep: %px\n", my_cachep);
    pr_info("my_cachep.object_size: %u\n", kmem_cache_size(my_cachep));

    for(i = 0; i<OBJ_NUM; i++){
        ms[i] = kmem_cache_alloc(my_cachep, GFP_KERNEL);
        pr_info("cache alloc: %px\n", ms[i]);
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


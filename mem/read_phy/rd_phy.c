/*
 * Test: 基于 memremap 和 ioremap 读取物理内存
 * 不知道有没有用
 *
 *
 * insmod rd_phy.ko pa=0x90ba003
 *
 * */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/io.h>

static long pa=0x1000000;
module_param(pa,long,0660);

long run(void){
    void *va;
    //va = ioremap(pa, PAGE_SIZE);
    va = memremap(pa, PAGE_SIZE, MEMREMAP_WB);
    if(!va)
        pr_info("[memremap] failed\n");
    else
        pr_info("[memremap] pa: %px, va: %px, content: %lx\n", (void *)pa, (void *)va, *(long *)va);

    va = ioremap(pa, PAGE_SIZE);
    if(!va)
        pr_info("[ioremap] failed\n");
    else
        pr_info("[ioremap]  pa: %px, va: %px, content: %lx\n", (void *)pa, (void *)va, *(long *)va);
    return 0;

}

static int __init km_init(void){
   run();
   return 0;
}

static void __exit km_exit(void)
{
    pr_info("Bye\n");
}

module_init(km_init);
module_exit(km_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("0.1");


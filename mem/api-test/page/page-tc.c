/*
 * API 测试：
 * - alloc_pages 根据 order  分配 struct page *
 * - alloc_page  分配 order 0 的 struct page *
 * - page_address 获取 struct *page 对应的 virtual address
 * - __get_free_pages: alloc_pages + page_address 返回 virtual address
 * - virt_to_page: virtual address 转 struct page *
 *
 * */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mm.h>

static int __init km_init(void){
    struct page * page;
    void *p;

    page = alloc_page(GFP_KERNEL);

    pr_info("page: %px, addr: %px, pfn: %px\n", 
            page, 
            page_address(page), 
            (void *)page_to_pfn(page));

    free_page((unsigned long)page_address(page));

    p = (void *)__get_free_pages(GFP_KERNEL, 0);
    pr_info("page: %px, addr: %px\n", 
            virt_to_page(p), 
            page_address(page));
    free_pages((unsigned long)p, 0);


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


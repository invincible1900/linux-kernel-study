#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mm.h>

#define X_ORDER 0

static int __init km_init(void){
    struct page *page[10];
    struct zone *zone;
    struct list_head *free_list, *node;
    int i;

    pr_info("km_init: %px\n", km_init);
    page[0] = alloc_pages(GFP_KERNEL, X_ORDER);
    zone = page_zone(page[0]);

    pr_info("page: %px, page_zone: %px\n", page[0], zone);

    pr_info("zone->free_area: %px, free_list: %px, nr_free: %ld\n", 
                &zone->free_area[X_ORDER], 
                zone->free_area[X_ORDER].free_list, 
                zone->free_area[X_ORDER].nr_free);

    for( i = 0; i<MIGRATE_TYPES; i++){
        free_list = &(zone->free_area[X_ORDER].free_list[i]);
        pr_info("free_list[%d]: %px\n", i, free_list);
        node = free_list->next;
        while(node != free_list){
            pr_info("free_list[%d]: %px, node: %px, next: %px\n", i, free_list, node, node->next);
            node = node->next;
        }
        pr_info("+++++++++++++++++++++++++++++++++++++\n");
    }
 
    for( i = 1; i<4; i++){
        page[i] = alloc_pages(GFP_KERNEL, X_ORDER);
        pr_info("page: %px, page_zone: %px\n", page[i], page_zone(page[i]));
    
    } 


    for( i = 0; i<4; i++){
        pr_info("page: %px, page_zone: %px\n", page[i], page_zone(page[i]));
        pr_info("%lx\n", (unsigned long)page[i]);
        free_pages((unsigned long)(page[i]), X_ORDER);
    }
#if 0
    pr_info("free_list[0]: %px\n", zone->free_area[X_ORDER].free_list[0].next);
    page = alloc_pages(GFP_KERNEL, X_ORDER);
    pr_info("page: %px\n", page);


    pr_info("free_list[0]: %px\n", zone->free_area[X_ORDER].free_list[0].next);
    page = alloc_pages(GFP_KERNEL, X_ORDER);
    pr_info("page: %px\n", page);
#endif
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


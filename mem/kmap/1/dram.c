#include <linux/module.h>	// for module_init() 
#include <linux/highmem.h>	// for kmap(), kunmap()

static int __init dram_init( void )
{
	unsigned long page_number, page_indent;
	unsigned long pa = 0x1000000;
	struct page *pp;
	void *va;

	pr_info("Hello\n");

	page_number = pa / PAGE_SIZE;
    page_indent = pa % PAGE_SIZE;

    pp = pfn_to_page( page_number);

    va = kmap( pp ) + page_indent;

	pr_info("va: %px, pa: %px\n", va, (void *)pa);

	kunmap(pp);
	return 0;

}

static void __exit dram_exit( void )
{
	pr_info("Bye\n");
}


MODULE_LICENSE("GPL");
module_init( dram_init );
module_exit( dram_exit );


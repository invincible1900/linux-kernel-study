/*
 * 创建并打印一个hlist
 *
 * 测试环境: Linux 5.8.0-38-generic #43~20.04.1-Ubuntu SMP Tue Jan 12 16:39:47 UTC 2021 x86_64 x86_64 x86_64 GNU/Linux
 *
 * */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/hashtable.h>
#include <linux/vmalloc.h>

#define HBITS 3

DEFINE_HASHTABLE(test_htable, HBITS);

void hlist_t(void){
	char *buff, *pos;
	int i;
	
	buff = vmalloc(4096);	
	if(!buff){
		pr_err("[!] vmalloc failed!\n");
		return;
	}

	pos = buff;

	for(i = 0; i < 1<<HBITS; i++){
		pos += sprintf(pos, "<%px> test_htable[%d].first: %px\n", &test_htable[i], i, test_htable[i].first);
	}
	
	pr_info("%s\n", buff);

}


static int __init hlist_t_init(void)
{
    printk(KERN_INFO "Hello hlist_t\n");
    hlist_t();
    return 0;
}

static void __exit hlist_t_exit(void)
{
    printk(KERN_INFO "Goodbye hlist_t\n");
}


module_init(hlist_t_init);
module_exit(hlist_t_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++0");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("0.1");

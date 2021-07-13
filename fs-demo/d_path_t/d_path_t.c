/*
*  
*  测试d_path和dentry_path_raw效果
*/


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
/*#include <linux/namei.h>
#include <linux/dcache.h>
#include <linux/path.h>
#include <linux/list.h>*/

#define SAFE(expr) \
	if(!expr){ \
		pr_info("%s-%d\n", __func__, __LINE__); \
		return -1; \
	} 

char * test_path[1] = {
	"/tmp/123/ls"
};



int d_path_t(void){
	struct file *filp;
	struct path *path;
	struct dentry *dentry;
	char *buf;
	char *p;
	// 打开一个文件用于获取一个file结构体
	pr_info( "[+] file path: %s\n", test_path[0]);
        filp = filp_open(test_path[0], O_RDONLY, 0);
	SAFE(filp)	
	path = &filp->f_path;
	dentry = filp->f_path.dentry;
	
	buf = (char*)__get_free_page(GFP_USER);
	p = d_path(path, buf, PAGE_SIZE);
		
	pr_info( "[+] d_path result: %s\n", p);
	
	memset(buf, 0, PAGE_SIZE);	
       
	 p = dentry_path_raw(dentry, buf, PAGE_SIZE);
	pr_info( "[+] dentry_path_raw result: %s\n", p);

	return 0;
}



static int __init d_path_t_init(void)
{
    printk(KERN_INFO "Hello\n");
    d_path_t();
    return 0;
}

static void __exit d_path_t_exit(void)
{
    printk(KERN_INFO "Goodbye\n");
}


module_init(d_path_t_init);
module_exit(d_path_t_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ikx");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("0.1");



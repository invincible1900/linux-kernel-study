/*
* 遍历task_struct 的 files 获取所有文件的file structure 
*/


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/namei.h>
#include <linux/dcache.h>
#include <linux/fs.h>
#include <linux/path.h>
#include <linux/list.h>
#include <linux/fdtable.h>

#define SAFE(expr) \
	if(!expr){ \
		pr_info("%s-%d\n", __func__, __LINE__); \
		return -1; \
	} 


int task2files(void){
	struct file *fp;
	struct fdtable *fdt;
	unsigned int i_fd = 3;
	char *buf = (char*)__get_free_page(GFP_USER);
	char *p = NULL;
	fdt = files_fdtable(current->files);
	pr_info("%s-%d, %u\n", __func__, __LINE__, fdt->max_fds);
	for(i_fd; i_fd < fdt->max_fds; i_fd++){
	pr_info("%s-%d\n", __func__, __LINE__);
		fp = fdt->fd[i_fd];
		if(!fp)
			break;
		memset(buf, 0, PAGE_SIZE);
		p = d_path(&fp->f_path, buf, PAGE_SIZE);
		pr_info( "[+] Full path: %s\n", p);

	}

	return 0;
}



static int __init task2files_init(void)
{
    printk(KERN_INFO "Hello\n");
    task2files();
    return 0;
}

static void __exit task2files_exit(void)
{
    printk(KERN_INFO "Goodbye\n");
}


module_init(task2files_init);
module_exit(task2files_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("0.1");


/*

include/linux/path.h

struct path {
        struct vfsmount *mnt;
        struct dentry *dentry;
} __randomize_layout;

*/
// int iterate_dir(struct file *file, struct dir_context *ctx)
/*
struct dir_context {
        filldir_t actor;
        loff_t pos;
};
*/

/*
 * File types
 *
 * NOTE! These match bits 12..15 of stat.st_mode
 * (ie "(i_mode >> 12) & 15").

#define DT_UNKNOWN      0
#define DT_FIFO         1
#define DT_CHR          2
#define DT_DIR          4
#define DT_BLK          6
#define DT_REG          8
#define DT_LNK          10
#define DT_SOCK         12
#define DT_WHT          14

*/

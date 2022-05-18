#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/vmalloc.h>

#define PATH  "/root/hello"

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
#define kernel_write(a, b, c, d) \
        kernel_write(a, b, c, d)
#define kernel_read(a, b, c, d) \
        kernel_read(a, b, c, d)
#else
#define kernel_write(a, b, c, d) \
        kernel_write(a, b, c, &d)

#define kernel_read(a, b, c, d) \
        kernel_read(a, b, c, &d)
#endif

void hexdump(const void* data, size_t size) {
    char buff[128];
	char ascii[17];
	size_t i, j;

    int len = 0;

	ascii[16] = '\0';
	for (i = 0; i < size; ++i) {
		// pr_info("%02X ", ((unsigned char*)data)[i]);
        len += sprintf(buff + len, "%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == size) {
			//pr_info(" ");
            len += sprintf(buff + len, " ");
			if ((i+1) % 16 == 0) {
				//pr_info("|  %s \n", ascii);
                len += sprintf(buff + len, "|  %s \n", ascii);
				pr_info("%s", buff);
                memset(buff, 0, sizeof(buff));
                len = 0;

			} else if (i+1 == size) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					//pr_info(" ");
                    len += sprintf(buff + len, " ");
				}
				for (j = (i+1) % 16; j < 16; ++j) {
					//pr_info("   ");
                    len += sprintf(buff + len, "   ");
				}
				// pr_info("|  %s \n", ascii);
                len += sprintf(buff + len, "|  %s \n", ascii);
				pr_info("%s", buff);
                memset(buff, 0, sizeof(buff));
                len = 0;
			}
		}
	}
}


int x_file_exists(char *path){
    struct file *filep;
    filep = filp_open(path, O_RDONLY, 0);
    if(IS_ERR(filep)){
            pr_err("[D] file %s not exist\n", path);
            return -1;
    }
    filp_close(filep, NULL);
    return 0;
}

/* 覆盖写  */
#define x_file_write x_file_trunc_write // alias
ssize_t x_file_trunc_write(char *path, char *buff, ssize_t buff_sz){
    struct file *filep;
    ssize_t ret;
    loff_t pos = 0;

    filep = filp_open(path, O_WRONLY|O_CREAT|O_TRUNC, 0666);

    if(IS_ERR(filep)){
        pr_err("[D] %s:%d\n", __func__, __LINE__);
        return -1;
    }

    ret = kernel_write(filep, buff, buff_sz, pos);

    filp_close(filep, NULL);

    return ret;
}

/* 追加写  */
ssize_t x_file_append_write(char *path, char *buff, ssize_t buff_sz){
    struct file *filep;
    ssize_t ret;
    loff_t pos = 0;

    filep = filp_open(path, O_WRONLY|O_APPEND, 0666);

    if(IS_ERR(filep)){
        pr_err("[D] %s:%d\n", __func__, __LINE__);
        return -1;
    }

    ret = kernel_write(filep, buff, buff_sz, pos);

    filp_close(filep, NULL);

    return ret;

}

ssize_t x_file_read(char *path, char *buff, size_t buff_sz){
    struct file *filep;
    ssize_t ret;
    loff_t pos = 0;

    filep = filp_open(path, O_RDONLY, 0);

    if(IS_ERR(filep)){
            pr_err("[D] %s:%d\n", __func__, __LINE__);
            return -1;
    }

    ret = kernel_read(filep, buff, buff_sz, pos);

    filp_close(filep, NULL);

    return ret;
}

/* 读整个文件  */
ssize_t x_file_read_whole(char *path, char *buff, size_t buff_sz){
    // TODO
    return 0;
}

static void rw_ufile(void){
    char *buff;
    char *path = PATH;
    ssize_t buff_sz = 100;
    ssize_t ret = 0;

    /* alloc buff  */
    buff = vmalloc(buff_sz);
    if(!buff){
        pr_err("[D] %s:%d\n", __func__, __LINE__);
        goto err;
    }

    /* File exists ? */
    ret = x_file_exists(path);
    if(ret < 0){
            sprintf(buff, "Hello\n");
            ret = x_file_write(path, buff, buff_sz);
            if(ret < 0){
                    goto err;
            }
    }

    /* File read */
    ret = x_file_read(path, buff, buff_sz);
    if(ret < 0){
        goto err;
    }

    /* Dump file content */
    hexdump(buff, buff_sz);

    /* File modify and write */
    memset(buff, 0, buff_sz);
    sprintf(buff, "%s, %d\n", __func__, __LINE__);
    ret = x_file_write(path, buff, buff_sz);
    if(ret < 0){
        goto err;
    }

err:
    if(buff){
        vfree(buff);
    }

    return;
}


static int __init rw_ufile_init(void)
{
    printk(KERN_INFO "Hello\n");
    rw_ufile();
    return 0;
}

static void __exit rw_ufile_exit(void)
{
    printk(KERN_INFO "Goodbye\n");
}


module_init(rw_ufile_init);
module_exit(rw_ufile_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("0.1");


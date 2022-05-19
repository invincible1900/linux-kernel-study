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



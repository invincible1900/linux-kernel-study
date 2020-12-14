#include <linux/module.h>
#include <linux/types.h>
#include <asm/byteorder.h>

union u_be{
	char a[2];
	__be16 b;
}ube;

union u_le{
	char a[2];
	__le16 b;
}ule;

static void endian(void){
	u32 a32 = 0x61626364;
	
	ube.b = 0x6162;	
	ule.b = 0x6162;	

	pr_info("ube.a[0]: %c, ule.a[0]: %c\n", ube.a[0], ule.a[0]);
	
	u32 tmp_le = cpu_to_le32(a32);
	u32 tmp_be = cpu_to_be32(a32);
	pr_info("cpu_to_le32: %x \n", tmp_le);
	pr_info("cpu_to_be32: %x\n", tmp_be);

	
}


static int __init endian_init(void)
{
    printk(KERN_INFO "Hello endian\n");
    endian();
    return 0;
}

static void __exit endian_exit(void)
{
    printk(KERN_INFO "Goodbye endian\n");
}


module_init(endian_init);
module_exit(endian_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("XDT");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("0.1");

/*
output: 
 
[  266.321302] Hello endian
[  266.321303] ube.a[0]: b, ule.a[0]: b
[  266.321304] cpu_to_le32: 61626364
[  266.321304] cpu_to_be32: 64636261
[  270.061899] Goodbye endian
 
*/

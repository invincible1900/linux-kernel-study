/*
 * Linux kernel rb-tree test
 *
 * Tested on: Linux 5.4.0-58-generic #64-Ubuntu SMP Wed Dec 9 08:16:25 UTC 2020 x86_64 x86_64 x86_64 GNU/Linux 
 *
 * */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/rbtree.h>
#include <linux/rbtree_augmented.h>

#define BUFF_SIZE 24 

struct planet {
	char name[BUFF_SIZE];
        int mass;
        struct rb_node rb_node;
};

struct rb_root planet_rb_root = RB_ROOT;

void insert_planet( struct planet * planet);
void print_planet_tree(struct rb_node *n);
struct planet *create_planet(char * name, int mass);

// 创建一个节点(planet)
struct planet *create_planet(char * name, int mass){
	struct planet *new_plt;  
	
	if(strlen(name) >= BUFF_SIZE){
		pr_info("[-] name too long\n");	
		return NULL;
	}	

	new_plt = vmalloc(sizeof(struct planet));

	strcpy(new_plt->name, name);
	
	new_plt->mass = mass;

	pr_info("%s created: %px\n", name, new_plt);

	return new_plt;
}

// 插入到红黑树(planet_rb_root)中
void insert_planet( struct planet * planet){
	struct rb_node **p = &planet_rb_root.rb_node;
	struct rb_node *parent = NULL;

	while(*p){
		struct planet *tmp_planet;
		parent = *p;
		tmp_planet = rb_entry(parent, struct planet, rb_node);
		if(planet->mass < tmp_planet->mass){
			p = &(*p)->rb_left;
		}else{
			p = &(*p)->rb_right;
		}
	}

	rb_link_node(&planet->rb_node, parent, p);
        rb_insert_color(&planet->rb_node, &planet_rb_root);
}

// 遍历红黑树并打印节点信息
void print_planet_tree(struct rb_node *n){
	struct planet *planet;
	char *p_buff =  vmalloc(4096);
	char *pos;

	if(!n){
		return;
	}	

	planet = rb_entry(n, struct planet, rb_node);

	pos = p_buff;
	pos += sprintf(pos, "<%px> planet start ****************************************\n", planet);
	pos += sprintf(pos, "<%px> planet->name: %s(%px)\n", &planet->name, planet->name, planet->name);
	pos += sprintf(pos, "<%px> planet->mass: %d\n", &planet->mass, planet->mass);
	pos += sprintf(pos, "<%px> planet->rb_node: \n", &planet->rb_node);

	pos += sprintf(pos, "<%px> \tplanet->rb_node.__rb_parent_color %lx, rb_parent: %px, rb_color: %s\n", 
				&planet->rb_node.__rb_parent_color, 
				planet->rb_node.__rb_parent_color, 
				rb_parent(&planet->rb_node), 
				rb_color(&planet->rb_node)? "black": "red");

	pos += sprintf(pos, "<%px> \tplanet->rb_node.rb_right %px\n", &planet->rb_node.rb_right, planet->rb_node.rb_right);
	pos += sprintf(pos, "<%px> \tplanet->rb_node.rb_left %px\n", &planet->rb_node.rb_left, planet->rb_node.rb_left);
	pos += sprintf(pos, "<%px> planet end  *****************************************\n\n", planet+1);

	pr_info("%s", p_buff);

	vfree(p_buff);

	if(n->rb_left){
		print_planet_tree(n->rb_left);
	}
	if(n->rb_right){
		print_planet_tree(n->rb_right);
	}

	return;	
}

void rbt(void){
	struct planet *earth, *mars, *saturn, *venus, *jupiter;
	
	earth = create_planet("Earth", 0);
	venus = create_planet("Venus", 1);
	saturn = create_planet("Saturn", 3);
	mars = create_planet("Mars", 4);
	jupiter = create_planet("Jupiter", 7);
	
	insert_planet(earth);
	insert_planet(venus);
	insert_planet(saturn);
	insert_planet(mars);
	insert_planet(jupiter);

	pr_info("\n<%px> planet_rb_root.rb_node: %px\n\n", &planet_rb_root.rb_node, planet_rb_root.rb_node);
	
	print_planet_tree(planet_rb_root.rb_node);	
}


static int __init rbt_init(void)
{
    printk(KERN_INFO "Hello rbt\n");
    rbt();
    return 0;
}

static void __exit rbt_exit(void)
{
    printk(KERN_INFO "Goodbye rbt\n");
}


module_init(rbt_init);
module_exit(rbt_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("Kernel xxx Module.");
MODULE_VERSION("0.1");


/*
 * Tested on: 
 * 	Linux 5.4.0-56-generic #62-Ubuntu SMP Mon Nov 23 19:20:19 UTC 2020 x86_64 x86_64 x86_64 GNU/Linux
 * */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/netlink.h>
#include <linux/kernel.h>
#include <net/genetlink.h>
#include "../common.h"

/*
API list: 

- genl_register_family
- genl_unregister_family

- genl_family
- genl_ops


- genl_info
- nlmsghdr
- genlmsghdr 
- nlattr

- genlmsg_data
- nla_data 

*/


static int mgnl_test_handler(struct sk_buff *skb, struct genl_info *info);

struct genl_ops mgnl_ops[] = {
	{
		.cmd = MGNL_TEST_CMD,
		.doit = mgnl_test_handler,
		.dumpit = NULL, 
	},
}; 


static struct genl_family mgnl_family = {
	.name = MGNL_FML_NAME,
	.version = 1,  // ?
	.maxattr = 256, // ?
	.netnsok = false,
	.module = THIS_MODULE,
	.ops = mgnl_ops,
	.n_ops = ARRAY_SIZE(mgnl_ops),
};


#if 0
static void dump_recv_info(struct sk_buff *skb, struct genl_info *info){
	char tmp[1024];
	pr_info("skb: %px, info: %px\n", skb, info);

	sprintf(tmp, "snd_seq: %d\nsnd_portid: %d\nnlhdr: %px\ngenlhdr: %px\nuserhdr: %px\nattrs: %px, attrs[1]: %px\n", 
		info->snd_seq,
		info->snd_portid,
		info->nlhdr,
		info->genlhdr,
		info->userhdr,
		info->attrs, info->attrs[1]
	);
	
	pr_info("genl_info: \n%s\n", tmp);
	
}
#endif

static int mgnl_test_handler(struct sk_buff *skb, struct genl_info *info){
	
	struct nlmsghdr *nlhdr;
	struct genlmsghdr *genlhdr;
	struct nlattr *nlh;
	char *msg;

#if 0
	dump_recv_info(skb, info);	
	
	nlhdr = nlmsg_hdr(skb);
	genlhdr = nlmsg_data(nlhdr);
	nlh = genlmsg_data(genlhdr);
	msg = nla_data(nlh);
	pr_info("nlhdr: %px, genlhdr: %px, nlh: %px, msg: %px", 
		nlhdr, genlhdr, nlh, msg);

        pr_info("[+] %u(%u) says %s \n", 
		info->snd_portid, current->tgid, 
                (char*)nla_data(info->attrs[1]));


#endif
	nlhdr = info->nlhdr;
	genlhdr = info->genlhdr;
	nlh = genlmsg_data(genlhdr);
	msg = nla_data(nlh);
	
        pr_info("[+] %s (%u, %u) says %s \n", current->comm,
    		info->snd_portid, current->tgid, msg);


        return 0;
}

int mgnl_init(void){
	int ret;
	ret = genl_register_family(&mgnl_family);
	if(ret < 0){
		pr_err("[-] genl_register_family error\n");
		return ret;
	}
	pr_info("[+] mgnl_init ok\n");
	return 0;
}

void mgnl_exit(void){
	genl_unregister_family(&mgnl_family);
	pr_info("[+] mgnl_exit ok\n");
}

module_init(mgnl_init);
module_exit(mgnl_exit);

MODULE_AUTHOR("X++D");
MODULE_LICENSE("GPL");


/*
 * Tested on:
 * 	Linux 5.4.0-56-generic #62-Ubuntu SMP Mon Nov 23 19:20:19 UTC 2020 x86_64 x86_64 x86_64 GNU/Linux
 * */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <net/sock.h>
#include <linux/netlink.h>

#define NETLINK_GROUP_ID 	1   
#define NETLINK_TEST		31
#define MAX_MSG_SIZE		256

static int msg_send_handler(void);
static void msg_recv_handler(struct sk_buff *skb);

struct sock *netlink_sock = NULL;

struct netlink_kernel_cfg cfg = {
	.groups = NETLINK_GROUP_ID, 
	.input = msg_recv_handler,
};

static int msg_send_handler(void){
	int msg_len;
	
	struct sk_buff *skb = NULL;
	struct nlmsghdr *nlh = NULL;
	char msg[MAX_MSG_SIZE] = {0, };
	pid_t pid = current->tgid;
	
	sprintf(msg, "Hello %d", pid);
	msg_len = strlen(msg) + 1;
	
	skb = nlmsg_new(msg_len, GFP_ATOMIC); 
	if(!skb){
		return -1;
	}

	nlh = nlmsg_put(skb, 0, 0, NETLINK_TEST, msg_len, 0);
	if(!nlh){
		nlmsg_free(skb);
		return -1;
	}

	memcpy(nlmsg_data(nlh), msg, msg_len);
	
	netlink_unicast(netlink_sock, skb, pid, MSG_DONTWAIT);	
	
	return 0;
}

static void msg_recv_handler(struct sk_buff *skb){

	struct nlmsghdr *nlh = NULL;
	char *recv_msg = NULL;

	if(skb->len >= nlmsg_total_size(0)){
		
		nlh = nlmsg_hdr(skb);
	
		recv_msg = nlmsg_data(nlh);	
		
		if(recv_msg){
			pr_info("[+] Received msg from user: %s\n ", recv_msg);	
		}

		msg_send_handler();
	}
}


static int m_nl_init(void){
	netlink_sock = (struct sock*)netlink_kernel_create(&init_net, NETLINK_TEST, &cfg);
	if(!netlink_sock){
		return -1;
	}
	pr_info("[D] m_nl_init\n");
	return 0;
}

static void m_nl_exit(void){
	if(netlink_sock){
		netlink_kernel_release(netlink_sock);
		netlink_sock = NULL;
	}
	pr_info("[D] m_nl_exit\n");
}

module_init(m_nl_init);
module_exit(m_nl_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("X++D");
MODULE_DESCRIPTION("netlink example");


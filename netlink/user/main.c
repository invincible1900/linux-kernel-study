#include <unistd.h>
#include <stdio.h>
#include <asm/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <string.h>
#include <stdlib.h>

#define NETLINK_GROUP_ID 	1
#define NETLINK_TEST    31
#define MAX_MSG_SIZE	256


/* 用于接收内核消息的缓冲区 */
typedef struct m_nlmsg {
	struct nlmsghdr nlh;
	char msg[MAX_MSG_SIZE];
} nl_msg;

int main(int argc, char *argv[]){

	int netlink_socket;
	int ret;	
	struct sockaddr_nl sa, da;
	struct nlmsghdr * nlh;
	nl_msg nl_recv_msg;
	int addr_len = sizeof(struct sockaddr_nl);	
	pid_t pid = getpid();
	memset(&sa, 0, sizeof(sa));
	memset(&da, 0, sizeof(da));
	
	/* 创建AF_NETLINK socket */	
	netlink_socket = socket(AF_NETLINK, SOCK_RAW, NETLINK_TEST);
	if(netlink_socket < 0){
		perror("[-] socket create error");
		exit(-1);	
	}

	//printf("[D] netlink_socket: %d\n", netlink_socket);

	/* 创建source address 并绑定socket  */	
	sa.nl_family = AF_NETLINK;
	sa.nl_pid = pid;  
	sa.nl_groups = NETLINK_GROUP_ID;
	
	ret = bind(netlink_socket, (struct sockaddr *) &sa, sizeof(sa));
	if(ret < 0){
		perror("[-] socket bind error");
		close(netlink_socket);
		exit(-1);	
	}	

	/* 创建 dest address (内核地址: nl_pid = 0) 用于收发数据 */
	da.nl_family = AF_NETLINK;
	da.nl_pid = 0;  
	da.nl_groups = NETLINK_GROUP_ID;

	/* 构造 nlmsg 发送数据 */	
	nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_MSG_SIZE));
	memset(nlh, 0, sizeof(struct nlmsghdr));
	nlh->nlmsg_len = NLMSG_SPACE(MAX_MSG_SIZE);
	nlh->nlmsg_flags = 0;
	nlh->nlmsg_type = 0;
	nlh->nlmsg_seq = 0;
	nlh->nlmsg_pid = pid;
	sprintf(NLMSG_DATA(nlh), "Hello, kernel! (from %d)", pid);

	/* 发送数据到内核  */	
	ret = sendto(netlink_socket, nlh, nlh->nlmsg_len, 0, (struct sockaddr *) &da, sizeof(da));
	if(ret < 0)
	{
		perror("[-] sendto error");
		close(netlink_socket);
		exit(-1);
	}

	/* 接收内核数据  */
	ret = recvfrom(netlink_socket, &nl_recv_msg, sizeof(nl_recv_msg), 0, (struct sockaddr *) &da, &addr_len);
	if(ret < 0)
	{
		perror("[-] recvfrom error");
		close(netlink_socket);
		exit(-1);
	}

	printf("[+] Received msg from kernel: %s\n", nl_recv_msg.msg);
	
	free((void *)nlh);
	
	return 0;
}



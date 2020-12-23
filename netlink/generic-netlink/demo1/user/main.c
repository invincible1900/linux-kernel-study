#include <stdio.h>
#include <unistd.h>
#include <netlink/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>
#include <linux/netlink.h>
#include "../common.h"

int main(){

	pid_t pid = getpid();

	char hello_msg[128];

	sprintf(hello_msg, "Hello kernel! (from %d)", pid);

	// Step 1. Create
	struct nl_sock *nl_socket = nl_socket_alloc();

	// Step 2. Connect
	genl_connect(nl_socket);

	int family_id = genl_ctrl_resolve(nl_socket, MGNL_FML_NAME);
	// printf("[D] family_id: %d\n", family_id);

	// Step 3. Message
	struct nl_msg * msg = nlmsg_alloc();

	genlmsg_put(
		msg,
		NL_AUTO_PORT, // port ?
		NL_AUTO_SEQ,
		family_id,
		0,	//  hdrlen ?
		NLM_F_REQUEST,  // flags ?
		MGNL_TEST_CMD,
		0	// version ?	
	);	

	nla_put_string(msg, 1, hello_msg);  // 1 ?

	// Step 4. Send	
	int ret = nl_send_auto(nl_socket, msg);
	if(ret < 0){
		return -1;
	}

	printf("[D] Send message to kernel: %s\n", hello_msg);

	// Step 5. Clean
	nlmsg_free(msg);
	nl_socket_free(nl_socket);
	
	return 1;
}

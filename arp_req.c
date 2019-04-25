#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <linux/if.h>
#include <netinet/if_ether.h>
#include <netpacket/packet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

int main()
{
	int fd = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ARP));

	struct ifreq ifr;
	char *a = "wlp1s0";
	size_t if_name_len = strlen(a);
	memcpy(ifr.ifr_name, a, if_name_len);

	ioctl(fd, SIOCGIFINDEX, &ifr);

	int ifindex = ifr.ifr_ifindex;
	const unsigned char ether_broad_cast_addr[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	struct sockaddr_ll addr = {0};
	addr.sll_family = AF_PACKET;
	addr.sll_ifindex = ifindex;
	addr.sll_halen = ETHER_ADDR_LEN;
	addr.sll_protocol = htons(ETH_P_ARP);
	memcpy(addr.sll_addr, ether_broad_cast_addr, ETHER_ADDR_LEN);

	struct ether_arp req;
	req.arp_hrd=htons(ARPHRD_ETHER);
	req.arp_pro=htons(ETH_P_IP);
	req.arp_hln=ETHER_ADDR_LEN;
	req.arp_pln=sizeof(in_addr_t);
	req.arp_op=htons(ARPOP_REQUEST);
	memset(&req.arp_tha,0,sizeof(req.arp_tha));
	const char *target_ip_string = "172.20.0.1";
	struct in_addr target_ip_addr={0};
	if(!inet_aton(target_ip_string, &target_ip_addr))
	{
	}
	memcpy(&req.arp_tpa, &target_ip_addr.s_addr, sizeof(req.arp_tpa));
	char *source_ip = "172.20.0.2";
	struct in_addr source_ipaddr = {0};
	if(!inet_aton(source_ip, &source_ipaddr));
	{
	}
	memcpy(&req.arp_spa, &source_ipaddr.s_addr, sizeof(req.arp_spa));
	req.arp_sha[0] = 0xf8;
	req.arp_sha[1] = 0xda;
	req.arp_sha[2] = 0x0c;
	req.arp_sha[3] = 0x53;
	req.arp_sha[4] = 0x3f;
	req.arp_sha[5] = 0x4f;
	sendto(fd,&req,sizeof(req),0,(struct sockaddr*)&addr,sizeof(addr));
}

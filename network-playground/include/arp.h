
#include <net/ip.h>

struct arp_table[]{

uint_t arp_hardware_type;
uint_t arp_protocol_type;
uint_t arp_hardware_length;
uint_t arp_protocol_length;
uint_t arp_operation;
uint_t arp_eth_source[6];
uint_t arp_ip_source;
uint_t arp_eth_dest[6];
uint_t arp_ip_dest;

}	
	
#define ARPHRD_ETHER 1
#define ARP_OP_REQUEST 1
#define ARP_OP_REPLY 2

void arp_add(in_addr_t ip, uint8_t *mac);
bool arp_remove(in_addr_t ip);
void arp_request(char *ip_dot);





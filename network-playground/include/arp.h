
#include <net/ip.h>

#define ARP_MAX 30

struct arp_entry{
    uchar *ipaddr;
    uchar *mac;
    arp_entry *next;
}

struct arp_packet{
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

syscall arp_resolve(in_addr_t ip, uint8_t *mac);
bool arp_remove(in_addr_t ip);
syscall arp_add(uchar *ip, uchar *mac);


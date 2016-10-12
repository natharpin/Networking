#define ARP_MAX 30

struct arp_entry{
    uchar *ipaddr;
    uchar *mac;
    arp_entry *next;
}arptab;

semaphore arpadd_sem;
semaphore arpdelete_sem;

typedef unsigned char uint8_t
typedef unsigned short int uint16_t
typedef unsigned int uint32_t

struct arp_packet{
    uint16_t arp_hardware_type;
    uint16_t arp_protocol_type;
    uint8_t arp_hardware_length;
    uint8_t arp_protocol_length;
    uint16_t arp_operation;
    uint8_t arp_eth_source[6];
    uint32_t arp_ip_source;
    uint8_t arp_eth_dest[6];
    uint32_t arp_ip_dest;
}	
	
#define ARPHRD_ETHER 1
#define ARP_OP_REQUEST 1
#define ARP_OP_REPLY 2

syscall arp_resolve(ulong ip, uchar *mac);
syscall arp_remove(ulong *ip);
syscall arp_add(uchar *ip, uchar *mac);


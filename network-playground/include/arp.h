#define ARP_MAX 30

struct arp_entry{
    uchar *ipaddr;
    uchar *mac;
    struct arp_entry *next;
};

struct arp_entry *arptab;

semaphore arpadd_sem;
semaphore arpdelete_sem;

typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned int uint32;

struct arp_packet{
    uint16 arp_hardware_type;
    uint16 arp_protocol_type;
    uint8 arp_hardware_length;
    uint8 arp_protocol_length;
    uint16 arp_operation;
    uint8 arp_eth_source[6];
    uint32 arp_ip_source;
    uint8 arp_eth_dest[6];
    uint32 arp_ip_dest;
};	
	
#define ARPHRD_ETHER 1
#define ARP_OP_REQUEST 1
#define ARP_OP_REPLY 2

syscall arp_resolve(ulong ip, uchar *mac);
syscall arp_remove(ulong *ip);
syscall arp_add(uchar *ip, uchar *mac);


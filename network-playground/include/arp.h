#define ARP_MAX 30

#ifndef _ARP_H_
#define _ARP_H_

struct arp_entry{
    char *ipaddr;
    char *mac;
    struct arp_entry *next;
};

struct arp_entry *arptab;

int arp_count;

command xsh_arp(int, char *[]);

semaphore arpadd_sem;
semaphore arpdelete_sem;

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;

#define ARPHRD_ETHER 1
#define ARP_OP_REQUEST 1
#define ARP_OP_REPLY 2

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
};

syscall arp_resolve(char ip, char *mac);
syscall arp_remove(char *ip);
syscall arp_add(char *ip, char *mac);

void arpinit(void);

command xsh_arp(int, char *[]);

#endif

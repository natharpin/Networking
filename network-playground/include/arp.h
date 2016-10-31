#define ARP_MAX 30

#ifndef _ARP_H_
#define _ARP_H_ "This is defined"

struct arp_entry{
    char *ipaddr;
    uchar *mac;
    struct arp_entry *next;
};

typedef struct arp_entry arpen;

extern arpen *arptab;

extern int arp_count;

extern semaphore arpadd_sem;
extern semaphore arpdelete_sem;

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;

#define ARPHRD_ETHER 1
#define ARP_OP_REQUEST 1
#define ARP_OP_REPLY 2

struct arp_packet{
    uint16_t hardware_type;
    uint16_t protocol_type;
    uint8_t hardware_length;
    uint8_t protocol_length;
    uint16_t operation;
    uchar eth_source[6];
    uchar ip_source[4];
    uchar eth_dest[6];
    uchar ip_dest[4];
};

syscall arp_resolve(char *ip, uchar *mac);
syscall arp_remove(char *ip);
syscall arp_add(char *ip, char *mac);

void arpinit(void);

//command xsh_arp(int, char *[]);

#endif /* _ARP_H_ */

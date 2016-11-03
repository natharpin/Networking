

#ifndef _PING_H_
#define _PING_H_

/* Ping Server Constants */
#define MAX_REQUESTS        20
#define MAX_READ_ATTEMPTS   10
#define REQUEST_PKTSZ       96
#define ARP_PKTSZ           42
#define PING_GATEWAY      "52:54:00:5A:4D:06\0" 

/* ICMP Packet Definitions */
#define ICMP_REQUEST 0x08       /* Ping Request     */
#define ICMP_REPLY   0x00       /* Ping Response    */

struct icmpgram
{
    uchar type;
    uchar code;
    ushort cksum;
    ushort id;
    ushort seq;
    uchar data[1];
};

extern semaphore ping_sem;

void pinginit(void);
syscall ping_request(char *);
syscall ping_recieve(void *);
syscall ping_reply(void *, int);

#endif /* _PING_H_ */

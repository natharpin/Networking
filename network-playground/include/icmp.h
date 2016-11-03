
#ifndef _ICMP_H_
#define _ICMP_H_

/* IP Packet Definitions */
#define IP_V4       4
#define IP_TTL      64
#define IP_VER      0xF0
#define IP_IHL      0x0F
#define IP_FLAG_DF  0x2

/* IP Header Length */
#define IPv4_SIZE   (12 + IP_ADDR_LEN * 2)

int icmpPrep(void *, ushort, char *, uchar *);
void setupEther(struct ethergram *, char *, uchar *, ushort, ushort);
int icmpPrint(void *, int);

#endif /* _ICMP_H_ */


#ifndef _IPV4_H_
#define _IPV4_H_

struct fragment{
    int seq;
    int length;
    uchar buffer[ETH_MTU];
    struct fragment *next;
};

extern struct fragment *fragHead;

void addFrag(int, int, void *);
void getFrag(uchar *buff);
void clearFrag(void);

syscall netWrite(void *, int, uchar *);
syscall ipWrite(void *, int, int, uchar *);
syscall ipv4Recv(void *, int);

#endif

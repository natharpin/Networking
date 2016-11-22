
#ifndef _IPV4_H_
#define _IPV4_H_

syscall netWrite(void *, int, uchar *);
syscall ipWrite(void *, int, int, uchar *);
syscall ipv4Recv(void *, int);

#endif

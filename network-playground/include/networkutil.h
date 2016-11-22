

#ifndef _NETWORKUTIL_H_
#define _NETWORKUTIL_H_

int firstOcc(char, char *, int );
bool isValidIpAddress(char *);
int ether_swap(void *);
int ipv4_swap(void *, int);
int getmac(void *);
int getip(void *);
void netDaemon(void);
void printEther(void *);

#endif

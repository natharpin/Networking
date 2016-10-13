#include <xinu.h>
#include <arp.h>

void arpDaemon(void);
syscall arpRecv(void *);

void arpinit()
{
	arptab = malloc(sizeof(struct arp_entry));
    arptab->ipaddr = (char *)malloc(sizeof(char) * 16);
    bzero((void *)arptab->ipaddr, sizeof(char) * 16);
    arptab->mac = (char *)malloc(sizeof(char) * 18);
    bzero((void *)arptab->mac, sizeof(char) * 18);
    arptab->next = NULL;
    
    arp_count = 0;

    arpadd_sem = semcreate(1);
    arpdelete_sem = semcreate(1);
    
    //TODO: set up arp daemon init

}

void arpDaemon(){
    //TODO: make arp daemon
    return;
}

syscall arpRecv(void *pkt)
{	
	return SYSERR;
}





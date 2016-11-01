#include <xinu.h>

void arpDaemon(void);
syscall arpRecv(struct ethergram *);

arpen *arptab;
int arp_count;
semaphore arpadd_sem;
semaphore arpdelete_sem;

void arpinit()
{
	arptab = malloc(sizeof(struct arp_entry));
    bzero((void *)arptab->ipaddr, IP_ADDR_LEN);
    bzero((void *)arptab->mac, ETH_ADDR_LEN);
    arptab->next = NULL;
    
    arp_count = 0;

    arpadd_sem = semcreate(1);
    arpdelete_sem = semcreate(1);
    
    //TODO: set up arp daemon init

    int arp_daemon = create((void *)arpDaemon, INITSTK, 1, "Arp Daemon", 0);
    if(!(isbadpid(arp_daemon)))
        ready(arp_daemon, FALSE);
}

void arpDaemon(){
    
    //wait for messages
    char *buff = (char *)malloc(PKTSZ);
    while(1){
        bzero((void *)buff, PKTSZ);
        read(ETH0, (void *)buff, PKTSZ);
                
        struct ethergram *frame = (struct ethergram *)buff;

        if(htons(frame->type) == ETYPE_ARP){
            arpRecv(frame);
        }
    }
}

void arp_reply(struct ethergram *frame){

    //get our mac address
    uchar *ourmac = (uchar *)malloc(ETH_ADDR_LEN);
    bzero((void *)ourmac, ETH_ADDR_LEN);
    control(ETH0, ETH_CTRL_GET_MAC, (ulong)ourmac, 0);
    
    //put our mac in the source and their mac in the destination at the frame level
    memcpy(frame->src, frame->dst, ETH_ADDR_LEN);
    memcpy(ourmac, frame->src, ETH_ADDR_LEN);
    
    //swap the ip source and destination
    struct arp_packet *pkt = (struct arp_packet *)frame->data;
    uchar *tempaddr = (uchar *)malloc(IP_ADDR_LEN);
    memcpy(tempaddr, pkt->ip_source, IP_ADDR_LEN);
    memcpy(pkt->ip_source, pkt->ip_dest, IP_ADDR_LEN);
    memcpy(pkt->ip_dest, tempaddr, IP_ADDR_LEN);
    
    //switch operation from request to reply
    pkt->operation = htons(ARP_OP_REPLY);
    
    //put our mac in the source and their mac in the destination at the packet level
    memcpy(pkt->eth_dest, pkt->eth_source, ETH_ADDR_LEN);
    memcpy(pkt->eth_source, ourmac, ETH_ADDR_LEN);
    
    write(ETH0, (void *)frame, PKTSZ);
}

syscall arpRecv(struct ethergram *frame)
{	
    
    struct arp_packet *pkt = (struct arp_packet *)frame->data;
    
    //TODO: save sender's mac
    
    if(!arp_exists(pkt->ip_source)){
        arp_add(pkt->ip_source, pkt->eth_source);
    }
    
    //TODO: if message is directed at us, reply
    
    uchar *ourip = (uchar *)malloc(IP_ADDR_LEN);
    dot2ip(nvramGet("lan_ipaddr\0"), ourip);
    
    if(!memcmp((void *)ourip, (void *)pkt->ip_dest, IP_ADDR_LEN) && pkt->operation == ARP_OP_REQUEST){
        arp_reply(frame);
    }

    free(ourip);

    return OK;
}





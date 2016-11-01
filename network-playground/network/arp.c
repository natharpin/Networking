#include <xinu.h>

void arpDaemon(void);
syscall arpRecv(void *);

arpen *arptab;
int arp_count;
semaphore arpadd_sem;
semaphore arpdelete_sem;

void arpinit()
{
	arptab = malloc(sizeof(struct arp_entry));
    arptab->ipaddr = (uchar *)malloc(IP_ADDR_LEN);
    bzero((void *)arptab->ipaddr, IP_ADDR_LEN);
    arptab->mac = (uchar *)malloc(ETH_ADDR_LEN);
    bzero((void *)arptab->mac, ETH_ADDR_LEN);
    arptab->next = NULL;
    
    arp_count = 0;

    arpadd_sem = semcreate(1);
    arpdelete_sem = semcreate(1);
    
    //TODO: set up arp daemon init

}

void arpDaemon(){
    //TODO: wait for messages
    
    char *buff = (char *)malloc(ETH_MAX_PKT_LEN);
    while(1){
        bzero((void *)buff, ETH_MAX_PKT_LEN);
        read(ETH0, (void *)buff, ETH_MAX_PKT_LEN);
        
        struct ethergram *frame = (struct ethergram *)buff;
        
        if(frame->type == ETYPE_ARP){
            arpRecv(frame);
        }
    }
}

/*    
    uint8_t eth_source[6];
    uint32_t ip_source;
    uint8_t eth_dest[6];
    uint32_t ip_dest;
*/

void arp_reply(struct ethergram *frame){

    //get our mac address
    uchar *ourmac = (uchar *)malloc(ETH_ADDR_LEN);
    bzero((void *)ourmac, ETH_ADDR_LEN);
    control(ETH0, ETH_CTRL_GET_MAC, (ulong)ourmac, 0);
    
    //put our mac in the source and their mac in the destination at the frame level
    memcpy(frame->src, frame->dst, ETH_ADDR_LEN);
    memcpy(ourmac, frame->src, ETH_ADDR_LEN);
    
    //TODO: switch operation
    pkt->operation = htons(ARP_OP_REPLY);
    
    //swap the ip source and destination
    struct arp_packet *pkt = (struct arp_packet *)frame->data;
    uchar temp_ipaddr = pkt->ip_source;
    pkt->ip_source = pkt->ip_dest;
    pkt->ip_dest = temp_ipaddr;
    
    //put our mac in the source and their mac in the destination at the packet level
    memcpy(pkt->eth_dest, pkt->eth_source, ETH_ADDR_LEN);
    memcpy(pkt->eth_source, ourmac, ETH_ADDR_LEN);
    
    write(ETH0, (void *)frame, ETH_MAX_PKT_LEN);
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

    if(!memcmp((void *)ourip, (void *)theirip, IP_ADDR_LEN)){
        arp_reply(frame);
    }

    return OK;
}





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

void num2mac(uint8_t eth_source, uchar *mac){
    
    uchar *temp = (uchar *)malloc(sizeof(uchar) * 18);
    bzero((void *)temp, sizeof(uchar) * 18);
    
    int i;
    for(i = 0; i < 6; i++){
        itoa(eth_source[i], temp, 16);
        strcat(temp, mac);
    }
}

/*    
    uint8_t eth_source[6];
    uint32_t ip_source;
    uint8_t eth_dest[6];
    uint32_t ip_dest;
*/

void arp_reply(struct ethergram *frame){

    uchar *ourmac = (uchar *)malloc(sizeof(uchar) * 18);
    bzero((void *)ourmac, 18 * sizeof(uchar));
    control(ETH0, ETH_CTRL_GET_MAC, (ulong)ourmac, 0);
    
    memcpy(frame->src, frame->dst, 18);
    memcpy(ourmac, frame->src, 18);
    
    //TODO: switch operation
    
    //TODO: fix
    struct arp_packet *pkt = (struct arp_packet *)frame->data;
    uchar temp_ipaddr = pkt->ip_source;
    pkt->ip_source = pkt->ip_dest;
    pkt->ip_dest = temp_ipaddr;
    
    uchar temp_eth[6];
    memcpy(temp_eth, pkt->eth_source, sizeof(uint8_t) * 6);
    mac2num(ourmac, pkt->eth_source);
    memcpy(pkt->eth_dest, temp_eth, sizeof(uint8_t) * 6);
    
    write(ETH0, (void *)frame, ETH_MAX_PKT_LEN);
}

syscall arpRecv(struct ethergram *frame)
{	
    struct arp_packet *pkt = (struct arp_packet *)frame->data;
    
    char *ipaddr = (char *)malloc(16);
    itoa(pkt->ip_source, ipaddr, 10);
    
    uchar *mac = (uchar *)malloc(sizeof(uchar) * 18);
    num2mac(pkt->eth_source, mac);
    
    //TODO: save sender's mac
    
    if(!arp_exists(ipaddr)){
        arp_add(ipaddr, mac);
    }
    
    //TODO: if message is directed at us, reply
    
    
    uchar *ourip = (uchar *)malloc(sizeof(uchar) * 18);
    dot2ip(nvramGet("lan_ipaddr\0"), ourip);
    
    char *tempip = (char *)malloc(18);
    itoa(pkt->ip_dest, tempip, 10);
    uchar *theirip = (uchar *)malloc(sizeof(uchar) * 18);
    dot2ip(tempip, theirip);
    
    if(!memcmp((void *)ourip, (void *)theirip, 18 * sizeof(uchar))){
        arp_reply(frame);
    }

    return OK;
}






#include <xinu.h>

syscall arp_request(uchar *ip, uchar *mac){

    uchar broadcast[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    //TODO: make the entire buffer that the frame fits in
    
    char *buff = malloc(PKTSZ);
    bzero((void *)buff, PKTSZ);

    //TODO: make the frame
    
    struct ethergram *frame = (struct ethergram *)buff;
    memcpy(frame->dst, broadcast, ETH_ADDR_LEN);
    uchar *tempmac = (uchar *)malloc(ETH_ADDR_LEN);
    bzero(tempmac, ETH_ADDR_LEN);
    control(ETH0, ETH_CTRL_GET_MAC, (ulong)tempmac, 0);
    memcpy(frame->src, tempmac, ETH_ADDR_LEN);
    frame->type = htons(ETYPE_ARP);
    
    //TODO: make the packet
    
    struct arp_packet *pkt = (struct arp_packet *)frame->data;
    pkt->hardware_type = htons(ARPHRD_ETHER);
    pkt->protocol_type = htons(0x0800);
    pkt->hardware_length = ETH_ADDR_LEN;
    pkt->protocol_length = IP_ADDR_LEN;
    pkt->operation = htons(ARP_OP_REQUEST);
    memcpy(pkt->eth_source, tempmac, ETH_ADDR_LEN);
    uchar *d2ip1 = (uchar *)malloc(IP_ADDR_LEN);
    dot2ip(nvramGet("lan_ipaddr\0"), d2ip1);
    memcpy(pkt->ip_source, d2ip1, IP_ADDR_LEN);
    bzero((void *)pkt->eth_dest, ETH_ADDR_LEN);
    memcpy(pkt->ip_dest, ip, IP_ADDR_LEN);
    
    //TODO: actually send the frame
    
    if(write(ETH0, (void *)buff, PKTSZ) == SYSERR){
        printf("Error in writing arp request to the network\n");
        return SYSERR;
    }
    
    return OK;
}

syscall arp_resolve_rec(char *ip, uchar *mac, int num){
    
    uchar *ipnums = (uchar *)malloc(IP_ADDR_LEN);
    dot2ip(ip, ipnums);
    
    //TODO: check arp table
    
    arpen *current = arptab;
    while(current->next != NULL){
        current = current->next;
        if(!memcmp(ipnums, current->ipaddr, IP_ADDR_LEN)){
            memcpy(mac, current->mac, ETH_ADDR_LEN);
            return OK; 
        }
    }
    
    if(num == 3){
        return SYSERR;
    }
    
    //else send
    arp_request(ipnums, mac);
    
    //Clean up resources used
    free(ipnums);
    
    sleep(1000);
    
    //call this again, incremented to hit the catch at num == 3
    return arp_resolve_rec(ip, mac, num + 1);
}

syscall arp_resolve(char *ip, uchar *mac){
    
    return arp_resolve_rec(ip, mac, 0);
}



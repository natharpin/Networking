
#include <xinu.h>

static const long hextable[] = {
   [0 ... 255] = -1,
   ['0'] = 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
   ['A'] = 10, 11, 12, 13, 14, 15,
   ['a'] = 10, 11, 12, 13, 14, 15      
};

long hexdec(unsigned const char *hex) {
   long ret = 0; 
   while (*hex && ret >= 0) {
      ret = (ret << 4) | hextable[*hex++];
   }
   return ret; 
}

void mac2num(uchar *mac, uint8_t *nums){
    int i = 0;
    int currentindex = 0;
    int charindex = 0;
    uchar currentchar[3];
    bzero(currentchar, 3 * sizeof(uchar));
    while(mac[i] != '\0'){
        while(mac[i] != ':'){
            currentchar[charindex] = mac[i];
            charindex++;
            i++;
        }
        charindex = 0;
        nums[currentindex] = (uint8_t)hexdec(currentchar);
        bzero(currentchar, 3 * sizeof(uchar));
        currentindex++;
    }
}

syscall arp_request(char *ip, uchar *mac){


    //TODO: make the entire buffer that the frame fits in
    char *buff = malloc(ETH_MAX_PKT_LEN);
    bzero((void *)buff, ETH_MAX_PKT_LEN);

    //TODO: make the frame
    struct ethergram *frame = (struct ethergram *)buff;
    memcpy(frame->dst, BROADCAST, 6);
    uchar *tempmac = (uchar *)malloc(ETH_ADDR_LEN);
    bzero(tempmac, 18 * sizeof(uchar));
    control(ETH0, ETH_CTRL_GET_MAC, (ulong)tempmac, 0);
    strncpy((char *)frame->src, (char *)tempmac, 18);
    frame->type = ETYPE_ARP;
    
    //TODO: make the packet
    
    struct arp_packet *pkt = (struct arp_packet *)frame->data;
    pkt->hardware_type = htons(1);
    pkt->protocol_type = htons(0x0800);
    pkt->hardware_length = ETH_ADDR_LEN;
    pkt->protocol_length = IP_ADDR_LEN;
    pkt->operation = htons(ARP_REQUEST);
    mac2num(tempmac, pkt->eth_source);
    uchar *d2ip1 = (uchar *)malloc(IP_ADDR_LEN);
    dot2ip(nvramGet("lan_ipaddr\0"), d2ip1);
    memcpy(pkt->ip_source, d2ip1m IP_ADDR_LEN);
    bzero((void *)pkt->eth_dest, 32);
    uchar *d2ip2 = (uchar *)malloc(IP_ADDR_LEN);
    dot2ip(nvramGet("lan_ipaddr\0"), d2ip2);
    pkt->ip_dest = atoi((char *)d2ip2);
    
    //memcpy(&frame->data[0], pkt, sizeof(arp_packet));
    
    //TODO: actually send the frame
    
    write(ETH0, (void *)buff, ETH_MAX_PKT_LEN);
    
    return OK;
}

syscall arp_resolve_rec(char *ip, uchar *mac, int num){
    //TODO: check arp table
    
    if(num == 3) return SYSERR;
    
    arpen *current = arptab;
    while(current->next != NULL){
        if(!strncmp(ip, current->ipaddr, 16)){
            strcpy((char *)mac, current->mac);
            return OK; 
        }
    }
    
    //TODO: else send
    
    arp_request(ip, mac);
    
    //TODO: wait one second
    
    sleep(1000);
    
    //TODO: call this again, num++
    
    return arp_resolve_rec(ip, mac, num + 1);
}

syscall arp_resolve(char *ip, uchar *mac){
    return arp_resolve_rec(ip, mac, 0);
}



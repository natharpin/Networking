
#include <xinu.h>

#define BROADCAST "FF:FF:FF:FF:FF:FF"

long strtol(uchar *string, char **pos, int base){
    return SYSERR;
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
        nums[currentindex] = (uint8_t)strtol(currentchar, NULL, 16);
        bzero(currentchar, 3 * sizeof(uchar));
        currentindex++;
    }
}

syscall arp_request(char *ip, uchar *mac){
    //TODO: make the frame
    struct ethergram *frame = (struct ethergram *)malloc(sizeof(struct ethergram));
    //frame->dst = (uchar *)malloc(sizeof(uchar) * 18);
    strncpy((char *)frame->dst, BROADCAST, 18);
    uchar *buff = (uchar *)malloc(sizeof(uchar) * 18);
    bzero(buff, 18 * sizeof(uchar));
    //frame->src = (uchar *)malloc(sizeof(uchar) * 18);
    control(ETH0, ETH_CTRL_GET_MAC, (ulong)buff, 0);
    strncpy((char *)frame->src, (char *)buff, 18);
    frame->type = ETYPE_ARP;
    
    //TODO: make the packet
    
    struct arp_packet *pkt = (struct arp_packet *)malloc(sizeof(struct arp_packet));
    pkt->hardware_type = 1;
    pkt->protocol_type = 0x0800;
    pkt->hardware_length = 18;
    pkt->protocol_length = 12;
    pkt->operation = 1;
    mac2num(buff, pkt->eth_source);
    uchar *d2ip1 = (uchar *)malloc(18);
    dot2ip(nvramGet("lan_ipaddr\0"), d2ip1);
    pkt->ip_source = atoi((char *)d2ip1);
    bzero((void *)pkt->eth_dest, 32);
    uchar *d2ip2 = (uchar *)malloc(18);
    dot2ip(nvramGet("lan_ipaddr\0"), d2ip2);
    pkt->ip_dest = atoi((char *)d2ip2);
    
    frame->data[0] = (char *)pkt;
    
    //TODO: wait for reply
    
    
    return SYSERR;
}

syscall arp_resolve(char *ip, uchar *mac){
    //TODO: check arp table
    
    //TODO: else send
    
    return SYSERR;
}



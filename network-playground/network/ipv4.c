#include <xinu.h>

syscall ipWrite(void *payload, int len, int type, uchar *ip){

    //The caller passes in the ICMP gram in the payload, 
    //we construct the ipv4 gram here, then pass it 
    //to netWrite which creates the ethergram

    //TODO: set up IPv4 gram header
    char *buff = (char *)malloc(sizeof(struct ipv4gram) + len);

    struct ipv4gram *ippkt = (struct ipv4gram *)buff;
    
    ippkt->proto = type;
    memcpy(ippkt->dst, ip, IP_ADDR_LEN);
    
    memcpy(ippkt->opts, payload, len);
    
    //TODO: send it to netWrite

    uchar mac[ETH_ADDR_LEN];
    arp_resolve(ip, mac);
    
    if(netWrite(ippkt, len, mac) == SYSERR){
        printf("Failed to netWrite!\n");
        return SYSERR;
    }

    free(buff);        
    return OK;
}

int ppktID = 0;

syscall netWrite(void *ipv4, int len, uchar *mac){

    char *buff = (char *)malloc(sizeof(struct ethergram) + sizeof(struct ipv4gram) + len);
    struct ethergram *ether = (struct ethergram *)buff;
    memcpy(ether->data, ipv4, len + sizeof(struct ipv4gram));
    
    struct ipv4gram *ippkt = (struct ipv4gram *)ether->data;

    printf("Sending packet type %x to %d.%d.%d.%d\n", ippkt->proto, ippkt->dst[0], ippkt->dst[1], ippkt->dst[2], ippkt->dst[3]);
    printf("Dst mac addr - %x:%x:%x:%x:%x:%x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    /* Set up the ipv4gram portion of packet */
    ippkt->ver_ihl = (IP_V4 << 4);
    ippkt->ver_ihl += (IPv4_SIZE / 4);
    ippkt->tos = 0;
    int length = len + sizeof(struct ipv4gram);
    ippkt->len = htons(length);
    ppktID++;
    ippkt->id = htons(ppktID);
    ippkt->flags_froff = (IP_FLAG_DF << 13);
    ippkt->flags_froff += 0;
    ippkt->flags_froff = htons(ippkt->flags_froff);
    ippkt->ttl = IP_TTL;
    ippkt->chksum = 0;
    getip(ippkt->src);
    //original checksum location
    
    // Set up ethergram
    memcpy(ether->dst, mac, ETH_ADDR_LEN);
    getmac(ether->src);
    ether->type = htons(ETYPE_IPv4);
              
    ippkt->chksum = checksum(ippkt, (4 * (ippkt->ver_ihl & IP_IHL)));

    printEther(ether);
    printIPv4(ippkt);
    printICMP(ippkt->opts);
                    
    if(write(ETH0, buff, (sizeof(struct ethergram) + sizeof(struct ipv4gram) + len)) == SYSERR){
        printf("Failed to write!\n");
    } else {
        printf("Wrote successfully!\n");
    }

    free(buff);
    return OK;
}

syscall ipv4Recv(void *frame, int length){

    printf("Entered ipv4Recv\n");

    struct ethergram *gram = (struct ethergram *)frame;
    struct ipv4gram *ippkt = (struct ipv4gram *)gram->data;
    
    printIPv4(ippkt);
    
    if(ippkt->ver_ihl != (IP_V4 << 4) + (IPv4_SIZE /4)){
        printf("ver_ihl is wrong!\n");
        return SYSERR;
    }
    /*
    if(ippkt->chksum != checksum((uchar *)ippkt, (4 * (ippkt->ver_ihl & IP_IHL)))){
        printf("Checksum failed!\n");
        return SYSERR;   
    }
    */
    
    uchar ip[IP_ADDR_LEN];
    getip(ip);
    
    printf("Getting packet type %x to %d.%d.%d.%d\n", ippkt->proto, ippkt->dst[0], ippkt->dst[1], ippkt->dst[2], ippkt->dst[3]);
    
    if(memcmp(ippkt->dst, ip, IP_ADDR_LEN) == 0){
        printf("Addressed to us\n");
        if(ippkt->proto == IPv4_PROTO_ICMP){
            struct icmpgram *icmp = (struct icmpgram *)ippkt->opts;
            printICMP(icmp);
            if(icmp->type == ICMP_REPLY){
                ping_recieve(frame);
            } else if(icmp->type == ICMP_REQUEST){
                ping_reply(frame, length);
            }
        }
    }
    else { printf("Not addressed to us\n"); }
    return OK;
}




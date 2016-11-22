#include <xinu.h>

int firstOcc(char c, char *str, int index){
    int i;
    for(i = index + 1; i < strlen(str); i++){
       if(c == str[i])
           return i;
    }
    return -1;
}

bool isValidIpAddress(char *ipaddr){
    int i;
    int period = 0;
    for(i = 0; i < 3; i++){
        if(firstOcc('.', ipaddr, period) != period && firstOcc('.', ipaddr, period) != -1)
            period = firstOcc('.', ipaddr, period);
        else
            return 0;
    }
    return 1;
}

int ether_swap(void *buf){
    struct ethergram *gram = (struct ethergram *)buf;
    uchar temp[ETH_ADDR_LEN];

    if (SYSERR == getmac(temp)){
        return SYSERR;
    }
    colon2mac(nvramGet("et0macaddr\0"), temp);

    memcpy(gram->dst, gram->src, ETH_ADDR_LEN);
    memcpy(gram->src, temp, ETH_ADDR_LEN);

    return OK;
}

int ipv4_swap(void *buf, int length){
    struct ipv4gram *gram = (struct ipv4gram *)buf;
    char temp[IP_ADDR_LEN];

    gram->chksum = htons(0x0000);

    memcpy(temp, gram->dst, IP_ADDR_LEN);
    memcpy(gram->dst, gram->src, IP_ADDR_LEN);
    memcpy(gram->src, temp, IP_ADDR_LEN);

    gram->chksum = checksum(buf, length);

    return OK;
}

int getmac(void *buf){

    uchar mac[ETH_ADDR_LEN];
    bzero((void *)mac, ETH_ADDR_LEN);
    control(ETH0, ETH_CTRL_GET_MAC, (ulong)mac, 0);

    if (NULL == mac){
        return SYSERR;
    }

    memcpy(buf, mac, ETH_ADDR_LEN);

    return OK;
}

int getip(void *buf){
    char *ip;

    ip = nvramGet("lan_ipaddr\0");

    if (NULL == ip){
        return SYSERR;
    }

    dot2ip(ip, (uchar *)buf);

    return OK;
}

void printEther(void *buff){

    struct ethergram *ether = (struct ethergram *)buff;

    printf("\n-- Printing ethergram --\n");
    printf("Address of ethergram: %x\n", ether);
    printf("Source address: %x:%x:%x:%x:%x:%x\n", ether->src[0], ether->src[1], ether->src[2], ether->src[3], ether->src[4], ether->src[5]);
    printf("Destination address: %x:%x:%x:%x:%x:%x\n", ether->dst[0], ether->dst[1], ether->dst[2], ether->dst[3], ether->dst[4], ether->dst[5]);
    switch(ntohs(ether->type)){
        case 0x0800:
            printf("Type: IPv4 packet\n");
            break;
        case 0x0806:
            printf("Type: ARP packet\n");
            break;
        default:
            printf("Not a recognized type!");
    }
    printf("Address of payload: %x\n\n", ether->data);
}

void printIPv4(void *buff){
    struct ipv4gram *ippkt = (struct ipv4gram *)buff;
    
    printf("\n-- Printing IPv4gram --\n");
    printf("Address of packet: %x\n", ippkt);
    printf("Ver_ihl: %x\n", ntohs(ippkt->ver_ihl));
    printf("tos: %d\n", ippkt->tos);
    printf("length: %d\n", ntohs(ippkt->len));
    printf("id: %d\n", ntohs(ippkt->id));
    printf("flags: %d\n", ntohs(ippkt->flags_froff));
    printf("ttl: %d\n", ippkt->ttl);
    printf("protocol: %d\n", ippkt->proto);
    printf("checksum: %d\n", ippkt->chksum);
    printf("IP source: %d.%d.%d.%d\n", ippkt->src[0], ippkt->src[1], ippkt->src[2], ippkt->src[3]);
    printf("IP destination: %d.%d.%d.%d\n", ippkt->dst[0], ippkt->dst[1], ippkt->dst[2], ippkt->dst[3]);
    printf("Address of payload: %x\n\n", ippkt->opts);
}

void printICMP(void *buff){
    struct icmpgram *icmp = (struct icmpgram *)buff;
    
    printf("\n-- Printing ICMPgram --\n");
    printf("Address of datagram: %x\n", icmp);
    printf("type: %d\n", icmp->type);
    printf("code: %d\n", icmp->code);
    printf("checksum: %d\n", icmp->cksum);
    printf("id: %d\n", icmp->id);
    printf("seq: %d\n\n", ntohs(icmp->seq));
}






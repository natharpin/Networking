#include <xinu.h>

syscall ipWrite(void *payload, int len, int type, char *ip){

    //The caller passes in the ICMP gram in the payload, 
    //we construct the ipv4 gram here, then pass it 
    //to netWrite which creates the ethergram

    //TODO: set up IPv4 gram header

    struct ipv4gram *ippkt = (struct ipv4gram *)malloc(sizeof(struct ipv4gram));

    /* Set up the ipv4gram portion of packet */
    ippkt->ver_ihl = (IP_V4 << 4);
    ippkt->ver_ihl += (IPv4_SIZE / 4);
    ippkt->tos = 0;
    ippkt->len = htons(len);
    ippkt->id = htons(id);
    ippkt->flags_froff = (IP_FLAG_DF << 13);
    ippkt->flags_froff += 0;
    ippkt->flags_froff = htons(ippkt->flags_froff);
    ippkt->ttl = IP_TTL;
    ippkt->proto = type;
    ippkt->chksum = 0;
    dot2ip(nvramGet("lan_ipaddr\0"), ippkt->src);
    dot2ip(ip, ippkt->dst);
    ippkt->chksum = checksum((uchar *)ippkt,
                            (4 * (ippkt->ver_ihl & IP_IHL)));
                            
    memcpy((void *)ippkt->opts, payload, len);
    
    //TODO: send it to netWrite

}

syscall netWrite(void *ipv4, uchar *mac){

    struct ipv4gram *ippkt = (struct ipv4gram *)ipv4;

}

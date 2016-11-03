#include <xinu.h>

int icmpPrep(void *buf, ushort id, char *dst, uchar *mac){
    struct ethergram *epkt = NULL;
    struct ipv4gram *ippkt = NULL;

    /* Assign the etherPkt to point to the passed buffer */
    epkt = (struct ethergram *)buf;
    ippkt = (struct ipv4gram *)epkt->data;

    /* Set up the ipv4gram portion of packet */
    ippkt->ver_ihl = (IP_V4 << 4);
    ippkt->ver_ihl += (IPv4_SIZE / 4);
    ippkt->tos = 0;
    ippkt->len = htons(REQUEST_PKTSZ - ETHER_SIZE);
    ippkt->id = htons(id);
    ippkt->flags_froff = (IP_FLAG_DF << 13);
    ippkt->flags_froff += 0;
    ippkt->flags_froff = htons(ippkt->flags_froff);
    ippkt->ttl = IP_TTL;
    ippkt->proto = IPv4_PROTO_ICMP;
    ippkt->chksum = 0;
    dot2ip(nvramGet("lan_ipaddr\0"), ippkt->src);
    dot2ip(dst, ippkt->dst);
    ippkt->chksum = checksum((uchar *)ippkt,
                            (4 * (ippkt->ver_ihl & IP_IHL)));


    /* Set up the ethergram portion of packet */
    getmac(epkt->src);

    memcpy(epkt->dst, mac, ETH_ADDR_LEN);

    epkt->type = htons(ETYPE_IPv4);

    return OK;
}

void setupEther(struct ethergram *request, char *dst, uchar *mac, ushort id, ushort seq){
    struct ipv4gram *ippkt;
    struct icmpgram *icmp;
    icmpPrep(request, id, dst, mac);
    ippkt = (struct ipv4gram *)request->data;
    icmp = (struct icmpgram *)ippkt->opts;
    icmp->type = ICMP_REQUEST;
    icmp->code = 0;
    icmp->id = 0;
    icmp->cksum = 0;
    icmp->seq = htons(seq);
    icmp->cksum = checksum(icmp ,(REQUEST_PKTSZ - ETHER_SIZE - IPv4_SIZE));
}

int icmpPrint(void *buf, int length){
    struct ipv4gram *ip = NULL;
    struct icmpgram *icmp = NULL;

    ip = (struct ipv4gram *)buf;
    icmp = (struct icmpgram *)ip->opts;

    printf("%d bytes from ", ntohs(ip->len));
    printf("(%d.%d.%d.%d) ",
           ip->src[0], ip->src[1], ip->src[2], ip->src[3]);
    printf("icmp_seq=%d ", ntohs(icmp->seq));
    printf("ttl=%d", ip->ttl);
    if (ICMP_REPLY != icmp->type){
        printf("\t(not reply)");
    }

    printf("\n");
    return OK;
}

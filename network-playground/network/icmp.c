#include <xinu.h>

void setup_icmpReq(struct icmpgram *request, ushort seq){
    request->type = ICMP_REQUEST;
    request->code = 0;
    request->id = 0;
    request->cksum = 0;
    request->seq = htons(seq);
    request->cksum = checksum(request ,(REQUEST_PKTSZ - ETHER_SIZE - IPv4_SIZE));
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

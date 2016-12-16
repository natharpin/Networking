#include <xinu.h>

typedef int boolean;
#define TRUE 1
#define FALSE 0

struct fragment *fragHead;
int ppktID = 0;

syscall ipWrite(void *payload, int len, int type, uchar *ip){

    //The caller passes in the ICMP gram in the payload, 
    //we construct the ipv4 gram here, then pass it 
    //to netWrite which creates the ethergram

    //TODO: set up IPv4 gram header
    char *buff = (char *)malloc(sizeof(struct ipv4gram) + len);
    struct ipv4gram *ippkt = (struct ipv4gram *)buff;

    ppktID++;

    boolean frag = FALSE;
    boolean lflag = FALSE;
    int totallen = 0;

    while(len > 0){
        
        bzero(ippkt, sizeof(struct ipv4gram));
    
        int templen = 0;
    
        if(len > ETH_MTU - sizeof(struct ipv4gram)){
            templen = ETH_MTU - sizeof(struct ipv4gram);
            frag = TRUE;
        }else if(frag && len <= ETH_MTU - sizeof(struct ipv4gram)){
            templen = len;
            lflag = TRUE;
        }else{
            templen = len;
        }
        
        ippkt->ver_ihl = (IP_V4 << 4);
        ippkt->ver_ihl += (IPv4_SIZE / 4);
        ippkt->tos = 0;
        
        int length = templen + sizeof(struct ipv4gram);
        ippkt->len = htons(length);
        ippkt->id = htons(ppktID);
        ippkt->proto = type;
        
        if(frag && !lflag){
            ippkt->flags_froff = (IPv4_FLAG_MF);
            ippkt->flags_froff += totallen / 8;
            ippkt->flags_froff = htons(ippkt->flags_froff);
        }else if(lflag){
            ippkt->flags_froff = (IPv4_FLAG_LF);
            ippkt->flags_froff += totallen / 8;
            ippkt->flags_froff = htons(ippkt->flags_froff);
        }else{
            ippkt->flags_froff = (IPv4_FLAG_DF);
            ippkt->flags_froff += 0;
            ippkt->flags_froff = htons(ippkt->flags_froff);
        }
        
        ippkt->ttl = IP_TTL;
        ippkt->chksum = 0;
        getip(ippkt->src);
        memcpy(ippkt->dst, ip, IP_ADDR_LEN);
        //Added the uchar * cast to checksum
        ippkt->chksum = checksum((uchar *)ippkt, (4 * (ippkt->ver_ihl & IP_IHL)));
    
        memcpy(ippkt->opts, payload, templen);
    
        //TODO: send it to netWrite
    
        uchar mac[ETH_ADDR_LEN];
        arp_resolve(ip, mac);
    
        if(netWrite(ippkt, templen, mac) == SYSERR){
            //printf("Failed to netWrite!\n");
            return SYSERR;
        }
        
        payload = (void *)(((long)payload) + templen);
        totallen += templen;
        len = len - templen;
    }
          
    free(buff);  
    return OK;
}

syscall netWrite(void *ipv4, int len, uchar *mac){

    char *buff = (char *)malloc(sizeof(struct ethergram) + sizeof(struct ipv4gram) + len);
    struct ethergram *ether = (struct ethergram *)buff;
    memcpy(ether->data, ipv4, len + sizeof(struct ipv4gram));
    
    struct ipv4gram *ippkt = (struct ipv4gram *)ether->data;

    //printf("Sending packet type %x to %d.%d.%d.%d\n", ippkt->proto, ippkt->dst[0], ippkt->dst[1], ippkt->dst[2], ippkt->dst[3]);
    //printf("Dst mac addr - %x:%x:%x:%x:%x:%x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    // Set up ethergram
    memcpy(ether->dst, mac, ETH_ADDR_LEN);
    getmac(ether->src);
    ether->type = htons(ETYPE_IPv4);              

    printEther(ether);
    printIPv4(ippkt);
    printICMP(ippkt->opts);
                    
    if(write(ETH0, buff, (sizeof(struct ethergram) + sizeof(struct ipv4gram) + len)) == SYSERR){
        //printf("Failed to write!\n");
    } 
    //else {
    //    printf("Wrote successfully!\n");
    //}

    free(buff);
    return OK;
}

int currID = -1;

syscall ipv4Recv(void *frame, int length){

    struct ethergram *gram = (struct ethergram *)frame;
    struct ipv4gram *ippkt = (struct ipv4gram *)gram->data;
    
    printIPv4(ippkt);
    
    if(ippkt->ver_ihl != (IP_V4 << 4) + (IPv4_SIZE /4)){
        return SYSERR;
    }
    
    uchar ip[IP_ADDR_LEN];
    getip(ip);
    
    //If the packet is addressed to us, deal with it
    if(memcmp(ippkt->dst, ip, IP_ADDR_LEN) == 0){
        //If the packet is fragmented, add it to the fragment list
        if(htons(ippkt->flags_froff) & IPv4_FLAG_MF){
            if(currID != ippkt->id){
                clearFrag();
                currID = ippkt->id;
            }
            int seq = ippkt->flags_froff & 0x1FFF;
            addFrag(seq, ippkt->len - sizeof(struct ipv4gram), ippkt->opts);
            return OK;
        } 
        //If the packet is the last one of a fragment chain, add it to the list
        else if(!(htons(ippkt->flags_froff) & 0xE000)){
            addFrag(ippkt->flags_froff, ippkt->len - sizeof(struct ipv4gram), ippkt->opts);
        }
        //If it is a ping packet, deal with it
        if(ippkt->proto == IPv4_PROTO_ICMP){
            struct icmpgram *icmp = (struct icmpgram *)ippkt->opts;
            //printICMP(icmp);
            if(icmp->type == ICMP_REPLY){
                ping_recieve(frame);
            } else if(icmp->type == ICMP_REQUEST){
                ping_reply(frame, length);
            }
        }
    }
    return OK;
}




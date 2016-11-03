#include <xinu.h>

void netDaemon(){
    
    //wait for messages
    int length;
    char *buff = (char *)malloc(PKTSZ);
    while(1){
        bzero((void *)buff, PKTSZ);
       
        while((length = read(ETH0, (void *)buff, PKTSZ)) == 0);
                
        struct ethergram *frame = (struct ethergram *)buff;

        if(htons(frame->type) == ETYPE_ARP){
            arpRecv(frame);
        } else if(htons(frame->type) == ETYPE_IPv4){
            struct ipv4gram *gram = (struct ipv4gram *)frame->data;
            if(gram->proto == IPv4_PROTO_ICMP){
                struct icmpgram *icmp = (struct icmpgram *)gram->opts;
                if(icmp->type == ICMP_REPLY){
                    ping_recieve(buff);
                } else if(icmp->type == ICMP_REQUEST){
                    ping_reply(buff, length);
                }
            }
        }
    }
}

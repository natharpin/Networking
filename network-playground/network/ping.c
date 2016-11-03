#include <xinu.h>

semaphore ping_sem;

void pinginit(){
    ping_sem = semcreate(0);
}

syscall ping_request(char *dst){

    uchar *mac = (uchar *)malloc(ETH_ADDR_LEN);
    bzero((void *)mac, ETH_ADDR_LEN);
    arp_resolve(dst, mac);

    char requestpkt[REQUEST_PKTSZ];
    char receivepkt[PKTSZ];
    struct ethergram *request;
    int length;
    int attempts;
    ushort id, seq;

    id = 0;
    seq = 0;
    request = (struct ethergram *)requestpkt;

    attempts = 0;
    while(attempts < 20){
	    attempts++;

    	bzero(request, REQUEST_PKTSZ);
    	setupEther(request, dst, mac, id, seq);

    	write(ETH0, (void *)request, REQUEST_PKTSZ);
    	sleep(1000);
    	seq++;
    }
    wait(ping_sem);
    return OK;
    
}

syscall ping_recieve(void *buff){

    char receivepkt[PKTSZ];
    struct ethergram *ether;
    struct ipv4gram *ippkt;
    struct icmpgram *icmp;
    struct arp_packet *argram;
    int length;
    int attempts, dropped;

    ether = (struct ethergram *)buff;
    ippkt = (struct ipv4gram *)ether->data;
    icmpPrint((void *)ippkt, ntohs(ippkt->len));

    dropped = 0;
    ether = (struct ethergram *)receivepkt;

    attempts = 1;
    while(attempts < 19){
	    attempts++;

    	bzero(ether, PKTSZ);
	
    	int i = 0;
    	int found = 0;
    	while(i < MAX_READ_ATTEMPTS && found == 0){
    	    while ((length = read(ETH0, (void *)ether, PKTSZ)) == 0)
                    ;
    
    	    if(ntohs(ether->type) == ETYPE_ARP){
                ether_swap(ether);
                argram = (struct arp_packet *)ether->data;
    	        if(ntohs(argram->operation) == ARP_OP_REQUEST){
    		        arp_reply(ether);
    		        write(ETH0, (void *)receivepkt, length);
    	        }
    	    } else {
    		    i++;
    	    }
    	    if(ntohs(ether->type) == ETYPE_IPv4){
    	        struct ipv4gram *ipgram = (struct ipv4gram *)ether->data;
    	        if(ipgram->proto == IPv4_PROTO_ICMP){
    		        struct icmpgram *icmpRep = (struct icmpgram *)ipgram->opts;
    		        if(icmpRep->type == ICMP_REPLY){
    			        found = 1;
    		            icmpPrint((void *)ipgram, ntohs(ipgram->len));
    		        }
    	        }
            }
    	}
    	if(found == 0) dropped++;
    }
    signal(ping_sem);
    printf("%d packets transmitted, %d packets dropped\n", attempts, dropped);
    return OK;
}

syscall ping_reply(void *buff, int length){
    struct ethergram *ether;
    struct ipv4gram *ipgram;
    struct icmpgram *icmp;

    ether = (struct ethergram *)buff;
	ipgram = (struct ipv4gram *)ether->data;
    icmp = (struct icmpgram *)ipgram->opts;
    
    ether_swap(ether);
    ipv4_swap(ipgram, ntohs(ipgram->len));
    icmp->cksum = 0;
    icmp->cksum = checksum(icmp ,(length - ETHER_SIZE - IPv4_SIZE));
    icmp->type = ICMP_REPLY;
    write(ETH0, (void *)buff, length);

    return OK;
}

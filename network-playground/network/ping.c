#include <xinu.h>

semaphore ping_sem;

void pinginit(){
    ping_sem = semcreate(0);
}

int pingID = 0;

syscall ping_request(char *dst){
    
    struct icmpgram *request = (struct icmpgram *)malloc(sizeof(struct icmpgram));

    int attempts = 0;
    ushort seq = 0;
    
    pingID++;
    
    while(attempts < 20){
	    attempts++;

    	bzero(request, sizeof(struct icmpgram));

        request->type = ICMP_REQUEST;
        request->code = 0;
        request->id = pingID;
        request->cksum = 0;
        request->seq = htons(seq);
        request->cksum = checksum(request ,(REQUEST_PKTSZ - ETHER_SIZE - IPv4_SIZE));

        uchar ip[IP_ADDR_LEN];
        dot2ip(dst, ip);

    	ipWrite((void *)request, sizeof(struct icmpgram), IPv4_PROTO_ICMP, ip);
    	sleep(1000);
    	seq++;
    }
    free(request);
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

    struct ethergram *ether = (struct ethergram *)buff;
	struct ipv4gram *ipgram = (struct ipv4gram *)ether->data;
    struct icmpgram *icmp = (struct icmpgram *)ipgram->opts;
    
    icmp->type = ICMP_REPLY;
    icmp->cksum = 0;
    icmp->cksum = checksum(icmp ,(length - ETHER_SIZE - IPv4_SIZE));
    ipWrite((void *)icmp, sizeof(struct icmpgram), IPv4_PROTO_ICMP, ipgram->src);

    return OK;
}

/*
Example ping from morbius for comparison to our output
-- Printing ethergram --
Address of ethergram: 80024ec0
Source address: 52:54:3:2:b1:6
Destination address: 0:1e:e5:85:ed:c5
Type: IPv4 packet
Address of payload: 80024ece

Recieved ipv4 message
Entered ipv4Recv

-- Printing IPv4gram --
Address of packet: 80024ece
Ver_ihl: 4500
tos: 0
length: 84
id: 0
flags: 16384
ttl: 64
protocol: 1
checksum: 54700
IP source: 192.168.6.10
IP destination: 192.168.6.121
Address of payload: 80024ee2

Getting packet type 1 to 192.168.6.121
Addressed to us

-- Printing ICMPgram --
Address of datagram: 80024ee2
type: 8
code: 0
checksum: 33884
id: 12993
seq: 15
*/

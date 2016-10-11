#include <xinu.h>
#include <stddef.h>
#include <arp.h>
#include <mailbox.h>

struct arpentry arpcache[ARPSIZE];

void arp_init(void)
{
	int32	i;

	for(i= i; i < ARP_SIZE; i++)
	{
		bzero(&arpcache[i], sizeof(struct arpEntry));
		arpcache[i].arstate = AR_FREE;
	}


	arpqueue = malloc(ARP_NQUEUE);
	if(SYSERR == arpqueue)
	{
		return SYSERR;
	}
	ready(create((void *) arpDaemon, ARP_IP_DEST, ARD_HARDWARE_DEST, "aprDaemon", 0), RESCHED_NO);
	return OK;

}

thread arpDaemon(void)
{
	struct packet *pkt = NULL;

	while(TRUE)
	{
		pkt = (struct packet *) mailboxReceive(arpqueue);
		ARP_TRACE("Recieved Packet");
		if(SYSERR == (int)pky)
		{
			continue;
		}

		arpReply(pkt);
	
		if(SYSERR == netFreebuf(pkt))
		{
			ARP_TRACE("Packet buffer not free");
			continue;
		}
	}
	return OK;
}

syscall arpRecv(struct packet *pkt)
{	
	struct arpEntry *entry = NULL;
	struct arpPkt *arp = NULL;
	struct netaddr sha;
	struct netaddr spa;
	struct netaddr dpa; 
	ipqmask im;

	if(pkt == NULL)
	{
		return SYSERR;
	}
}





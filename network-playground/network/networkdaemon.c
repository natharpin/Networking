#include <xinu.h>

void netDaemon(){
    
    //wait for messages
    int length;
    char *buff = (char *)malloc(PKTSZ);
    while(1){
        bzero((void *)buff, PKTSZ);
       
        while((length = read(ETH0, (void *)buff, PKTSZ)) == 0);
                
        struct ethergram *frame = (struct ethergram *)buff;

        //printEther(buff);

        if(htons(frame->type) == ETYPE_ARP){
            arpRecv(frame);
        } else if(htons(frame->type) == ETYPE_IPv4){
            printf("Recieved ipv4 message\n");
            ipv4Recv(frame, length);
        }
    }
}

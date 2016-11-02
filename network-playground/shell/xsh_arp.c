
#include <xinu.h>

#define H1 "IP addresses"
#define H2 "MAC addresses"

bool isValidIpAddress(char *);

void printarp(arpen *en){
    printf("%d.%d.%d.%d\t%X:%X:%X:%X:%X:%X\n", en->ipaddr[0], en->ipaddr[1], en->ipaddr[2], en->ipaddr[3], en->mac[0], en->mac[1], en->mac[2], en->mac[3], en->mac[4], en->mac[5]);
}

command xsh_arp(int nargs, char *args[]){

	if(nargs == 2) {
		if(!strncmp(args[1], "-n", 2)) {
		    printf("%s\t%s\n", H1, H2);
			int i;
			arpen *current = arptab;
			for(i = 0; i < arp_count; i++) {
				current = current->next;
			    printarp(current);
			}			
		} else if(isValidIpAddress(args[1])) {
		    uchar *mac = (uchar *)malloc(ETH_ADDR_LEN);
			if(arp_resolve(args[1], mac) != SYSERR){
			    printf("%s added to the arp table\n", args[1]);
			    uchar *ipaddr = (uchar *)malloc(IP_ADDR_LEN);
			    dot2ip(args[1], ipaddr);
			    arp_add(ipaddr, mac);
			} else {
			    printf("Could not reach %s\n", args[1]);
			}
		} else {
			printf("Incorrect formatting - 1 argument: apr [-n] [IP-address]\n");
		    return SYSERR;
		}
	}	
	else if(nargs == 3)	{
		if(!strncmp(args[1], "-d", 2)) {
			if(isValidIpAddress(args[2])) {
			    uchar *ipaddr = (uchar *)malloc(IP_ADDR_LEN);
			    dot2ip(args[2], ipaddr);
                            int result = arp_remove(ipaddr);
			    if(result == 2)
			        printf("Successful removal of %s\n", args[2]);
                            else if(result == 1)
                                printf("Unable to delete address because it does not exist in arp table\n");
                            else if(result == 0)
                                printf("Unable to delete address because the arp table is empty");
			} else {	
				printf("Incorrect IP Address formatting [xxx.xxx.xxx.xxx]\n");	
		        return SYSERR;
			}
		}
	} else {		//Arp command called with improper flags
		printf("Incorrect formatting: arp [-n] [-d IP-address] [IP-address]\n");
		return SYSERR;
	} 
    return OK;
}

int firstOcc(char c, char *str, int index){
    int i;
    for(i = index + 1; i < strlen(str); i++){
       if(c == str[i])
           return i;
    }
    return -1;
}

bool(isValidIpAddress(char *ipaddr)){
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




#include <xinu.h>

#define H1 "IP addresses"
#define H2 "MAC addresses"

bool isValidIpAddress(char *);

command xsh_arp(int nargs, char *args[]){

	if(nargs == 2) {
		if(!strncmp(args[1], "-n", 2)) {
		    printf("%s\t%s\n\r", H1, H2);
			int i;
			arpen *current = arptab;
			for(i = 0; i < arp_count; i++) {
				current = current->next;
				printf("%s\t%s",current->ipaddr, current->mac);
			}			
		} else if(isValidIpAddress(args[1])) {
		    uchar *mac = (uchar *)malloc(ETH_ADDR_LEN);
			if(arp_resolve(args[1], mac) != SYSERR){
			    printf("%s added to the arp table\n\r", args[1]);
			    uchar *ipaddr = (uchar *)malloc(IP_ADDR_LEN);
			    dot2ip(args[1], ipaddr);
			    arp_add(ipaddr, mac);
			} else {
			    printf("Could not reach %s\n\r", args[1]);
			}
		} else {
			printf("Incorrect formatting - 1 argument: apr [-n] [IP-address]\n\r");
		    return SYSERR;
		}
	}	
	else if(nargs == 3)	{
		if(!strncmp(args[1], "-d", 2)) {
			if(isValidIpAddress(args[2])) {
			    uchar *ipaddr = (uchar *)malloc(IP_ADDR_LEN);
			    dot2ip(args[2], ipaddr);
				if(arp_remove(ipaddr) == OK)
				    printf("Successful removal of %s\n\r", args[2]);
			} else {	
				printf("Incorrect IP Address formatting [xxx.xxx.xxx.xxx]\n\r");	
		        return SYSERR;
			}
		}
	} else {		//Arp command called with improper flags
		printf("Incorrect formatting: arp [-n] [-d IP-address] [IP-address]\n\r");
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



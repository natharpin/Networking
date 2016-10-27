
#include <xinu.h>

#define H1 "IP addresses"
#define H2 "MAC addresses"

bool isValidIpAddress(char *);

command xsh_arp(int nargs, char *args[]){

    char *ip = (char *)malloc(sizeof(char) * 16);
    strncpy(ip, args[1], 16);
    printf("%s\r\n", ip);
    
    
    uchar *ip2 = (uchar *)malloc(sizeof(uchar) * 16);
    dot2ip(args[1], ip2);
    printf("%d\r\n", ip2);
    

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
			//arpResolve with args[1], buffer of size 18.
  			//arp_add(args[1], buffer of size 18) 
  			//print success.
		} else {
			printf("Incorrect formatting - 1 argument: apr [-n] [IP-address]\n\r");
		    return SYSERR;
		}
	}	
	else if(nargs == 3)	{
		if(!strncmp(args[1], "-d", 2)) {
			if(isValidIpAddress(args[2])) {
				if(arp_remove(args[2]) == OK)
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
        printf("%d", period);
        if(firstOcc('.', ipaddr, period) != period && firstOcc('.', ipaddr, period) != -1)
            period = firstOcc('.', ipaddr, period);
        else
            return 0;
    }
    return 1;
}



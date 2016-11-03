#include <xinu.h>


command xsh_ping(int nargs, char *args[]){

	if(nargs == 2) {
        if(isValidIpAddress(args[1])){
            ping_request(args[1]);
        } else {	
	        printf("Incorrect IP Address formatting [xxx.xxx.xxx.xxx]\n");	
	        return SYSERR;
        }

	}
	if(nargs == 1) {
		printf("Incorrect formatting - Too few arguments: ping [IP-address]\n\r");
		return SYSERR;
	}

    return OK;
}




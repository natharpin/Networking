#include <xinu.h>


command xsh_ping(int nargs, char *args[]){

	if(nargs == 2) {


	}
	if(nargs == 1) {
		printf("Incorrect formatting - Too few arguments: ping [IP-address]\n\r");
		return SYSERR;
	}

}




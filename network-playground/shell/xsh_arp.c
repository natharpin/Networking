
#include <xinu.h>

bool isValidIpAddress(char *);

command xsh_arp(int nargs, char *args[])
{
	if(nargs == 1)
	{
		if(strncmp(args[1], "-n", 2))
		{
			int i;
			arpen *current = arptab;
			for(i = 0; i < arp_count; i++)
			{
				current = current->next;
				printf("%s\t%s",current->ipaddr, current->mac);
			}			
		}
		else if(isValidIpAddress(args[1]))
		{
			/*arpResolve with args[1], buffer of size 18.
 * 			arp_add(args[1], buffer of size 18) 
 * 			print success.*/
		} 
		else
		{
			printf("Incorrect formatting - 1 argument: apr [-n] [IP-address]");
		    return SYSERR;
		}
	}	
	else if(nargs == 2)
	{
		if(strncmp(args[1], "-d", 2)) 
		{
			if(isValidIpAddress(args[2]))
			{
				arp_remove(args[2]);
			}
			else
			{	
				printf("Incorrect IP Address formatting [xxx.xxx.xxx.xxx]");	
		        return SYSERR;
			}
		}
	}
	else		//Arp command called with improper flags
	{
		printf("Incorrect formatting: arp [-n] [-d IP-address] [IP-address].");
		return SYSERR;
	}
    return OK;
}

int firstOcc(char c, char *str, int index){
    int i;
    for(i = index; i < strlen(str); i++){
       if(c == str[i])
           return i;
    }
    return -1;
}

bool(isValidIpAddress(char *ipaddr)){
    int i;
    int colon = 0;
    for(i = 0; i < 3; i++){
        if(firstOcc(':', ipaddr, colon) != colon && firstOcc(':', ipaddr, colon) != -1)
            colon = firstOcc(':', ipaddr, colon);
        else
            return 0;
    }
    return 1;
}



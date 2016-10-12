command xsh_test(int nargs, char *args[])
{
	if(nargs == 1)
	{
		if(args[1] == "-n")
		{
			int i;
			struct arp_entry *current = arptab;
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
		}
	}	
	else if(nargs == 2)
	{
		if(args[1] == "-d") 
		{
			if(isValidIpAddress(args[2]))
			{
				arp_delete(args[2]);
			}
			else
			{	
				printf("Incorrect IP Address formatting [xxx.xxx.xxx.xxx]");	
			}
		}
	}
	else		//Arp command called with improper flags
	{
		printf("Incorrect formatting: arp [-n] [-d IP-address] [IP-address].");
	}

}



bool(isValidIpAddress(char *ipaddr))
{
	struct sockaddr_in *sa
	int result = inet_pton(AF_INET, ipaddr, &(sa.sin_addr));
	return result != 0;

}



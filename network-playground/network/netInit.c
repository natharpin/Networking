/**
 * @file netInit.c
 * @provides netInit.
 *
 */
/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

/**
 * Initialize network interface.
 */
void netInit(void)
{
    arpinit();
    pinginit();
    
    fragHead = (struct fragment *)malloc(sizeof(struct fragment));
    fragHead->seq = -1;
    fragHead->next = NULL;
    
    int net_daemon = create((void *)netDaemon, INITSTK, 1, "Network Daemon", 0);
    if(!(isbadpid(net_daemon)))
        ready(net_daemon, FALSE);

    open(ETH0);
    return;
}

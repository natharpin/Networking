/**
 * @file netInit.c
 * @provides netInit.
 *
 */
/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>
#include <arp.h>
#include <string.h>

/**
 * Initialize network interface.
 */
void netInit(void)
{

    arptab = malloc(sizeof(struct arp_entry));
    arptab->ipaddr = (uchar *)malloc(sizeof(uchar) * 16);
    bzero((void *)arptab->ipaddr, sizeof(uchar) * 16);
    arptab->mac = (uchar *)malloc(sizeof(uchar) * 18);
    bzero((void *)arptab->mac, sizeof(uchar) * 18);
    arptab->next = NULL;

    arpadd_sem = semcreate(1);
    arpdelete_sem = semcreate(1);

    open(ETH0);
    return;
}

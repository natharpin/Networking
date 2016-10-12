/**
 * @file netInit.c
 * @provides netInit.
 *
 */
/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>
#include <arp.h>

/**
 * Initialize network interface.
 */
void netInit(void)
{

    arptab = malloc(sizeof(struct arp_entry));
    struct arp_entry *arpInit = &arptab;
    arpInit->ip = calloc(16, sizeof(uchar));
    arpInit->mac = calloc(18, sizeof(uchar));
    arpInit->next = NULL;

    arpadd_sem = semcreate(1);
    arpdelete_sem = semcreate(1);

    open(ETH0);
    return;
}

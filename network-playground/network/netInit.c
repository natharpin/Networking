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

    open(ETH0);
    return;
}

#include <xinu.h>
#include <stddef.h>
#include <string.h>
#include <arp.h>

int arp_count = 0;

syscall arp_add(uchar *ip, uchar *mac){

    wait(arpadd_sem);
    struct arp_entry *current = &arptab;
    if(count >= ARP_MAX){
        struct arp_entry *replace = arptab.next->next;
        free((void *)arptab.next);
        arptab.next = replace;
        arp_count--;
    }
    while(current->next != NULL){
        current = current->next;
    }
    arp_count++;
    current->next = malloc(sizeof(struct arp_entry));
    strcpy(current->next->ip, ip);
    strcpy(current->next->mac, mac);
    current->next->next = NULL;
    return OK;
    signal(arpadd_sem);
}

syscall arp_remove(in_addr_t ip){
    return SYSERR;
}

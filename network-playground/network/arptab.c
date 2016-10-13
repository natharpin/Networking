#include <xinu.h>
#include <arp.h>

syscall arp_add(char *ip, char *mac){
    wait(arpadd_sem);
    arpen *current = arptab;
    if(arp_count >= ARP_MAX){
        arpen *replace = arptab->next->next;
        free((void *)arptab->next);
        arptab->next = replace;
        arp_count--;
    }
    while(current->next != NULL){
        current = current->next;
    }
    arp_count++;
    current->next = malloc(sizeof(struct arp_entry));
    strcpy(current->next->ipaddr, ip);
    strcpy(current->next->mac, mac);
    current->next->next = NULL;
    return OK;
    signal(arpadd_sem);
}

syscall arp_remove(char *ip){
    wait(arpdelete_sem);
    if(arp_count == 0)
        return OK;
    arpen *current = arptab;
    arpen *prev = arptab;
    uchar *ipaddr = (uchar *)malloc(sizeof(uchar *) * 16);
    //itoa(ip, ipaddr, 10);
    uchar *buff = (uchar *)malloc(sizeof(uchar *) * 16);
    while(current->next != NULL){
        prev = current;
        current = current->next;
        dot2ip(current->ipaddr, buff);
        dot2ip(ip, ipaddr);
        if(memcmp(buff, ipaddr, 16 * sizeof(uchar *))){
            prev->next = current->next;
            free((void *)current);
            break;
        } else {
            bzero((void *)buff, sizeof(char *) * 16);
        }
    }
    free((void *)ipaddr);
    free((void *)buff);
    signal(arpdelete_sem);
    return OK;
}

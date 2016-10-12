#include <xinu.h>
#include <stddef.h>
#include <string.h>
#include <arp.h>

int arp_count = 0;

syscall arp_add(uchar *ip, uchar *mac){
    wait(arpadd_sem);
    struct arp_entry *current = arptab;
    if(count >= ARP_MAX){
        struct arp_entry *replace = arptab->next->next;
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

syscall arp_remove(in_addr_t ip){
    wait(arpdelete_sem);
    if(arp_count == 0)
        return OK;
    struct arp_entry *current = arptab;
    struct arp_entry *prev = arptab;
    char *ipaddr = (char *)malloc(sizeof(char *) * 16);
    itoa(ip, ipaddr, 10);
    char *buff = malloc(sizeof(char *) * 16);
    while(current->next != NULL){
        prev = current;
        current = current->next;
        if(strncmp(dot2ip(current->ipaddr, buff), ipaddr, 16)){
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

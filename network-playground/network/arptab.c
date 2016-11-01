#include <xinu.h>

syscall arp_add(uchar *ip, uchar *mac){

    if(arp_exists(ip)){ return OK; }

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
    arpen *new_entry = (arpen *)malloc(sizeof(arpen));
    memcpy(new_entry->ipaddr, ip, IP_ADDR_LEN);
    memcpy(new_entry->mac, mac, ETH_ADDR_LEN);
    new_entry->next = NULL;
    current->next = new_entry;
    signal(arpadd_sem);
    return OK;
}

int arp_exists(uchar *ip){

    arpen *current = arptab;
    
    while(current->next != NULL){
        current = current->next;
        if(!memcmp(ip, current->ipaddr, IP_ADDR_LEN)){
            return TRUE;
        }
    }
    return FALSE;
}

syscall arp_remove(uchar *ip){
    wait(arpdelete_sem);
    int result = 0;
    if(arp_count == 0)
        return 1;
    arpen *current = arptab;
    arpen *prev = arptab;
    while(current->next != NULL){
        prev = current;
        current = current->next;
        if(!memcmp(ip, current->ipaddr, IP_ADDR_LEN)){
            if(current->next == NULL){
                prev->next = NULL;
            } else {
                prev->next = current->next;
            }
            free((void *)current);
            result = 2;
            break;
        }
        result = 1;
    }
    arp_count--;
    signal(arpdelete_sem);
    return result;
}

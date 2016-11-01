#include <xinu.h>

syscall arp_add(uchar *ip, uchar *mac){

    printf("entered arp add\n\r");
    sleep(1000);

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
    memcpy(new_entry, ip, IP_ADDR_LEN);
    memcpy(new_entry, mac, IP_ADDR_LEN);
    new_entry->next = NULL;
    current->next = new_entry;
    signal(arpadd_sem);
    return OK;
}

int arp_exists(uchar *ip){

    printf("checking if %d.%d.%d.%d exists\n\r", ip[0],ip[1],ip[2],ip[3]);
    sleep(1000);

    arpen *current = arptab;
    
    printf("current ip = %d.%d.%d.%d\n\r", current->ipaddr[0],current->ipaddr[1],current->ipaddr[2],current->ipaddr[3]);
    sleep(1000);    
    printf("current mac = %X:%X:%X:%X:%X:%X\n\r", current->mac[0],current->mac[1],current->mac[2],current->mac[3],current->mac[4],current->mac[5]);
    sleep(1000);
    
    while(current->next != NULL){
        current = current->next;
        if(!memcmp(ip, current->ipaddr, IP_ADDR_LEN)){
            return TRUE;
        }
        printf("current ip = %d.%d.%d.%d\n\r", current->ipaddr[0],current->ipaddr[1],current->ipaddr[2],current->ipaddr[3]);
        sleep(1000);    
        printf("current mac = %X:%X:%X:%X:%X:%X\n\r", current->mac[0],current->mac[1],current->mac[2],current->mac[3],current->mac[4],current->mac[5]);
        sleep(1000);

    }
    return FALSE;
}

syscall arp_remove(uchar *ip){
    wait(arpdelete_sem);
    if(arp_count == 0)
        return OK;
    arpen *current = arptab;
    arpen *prev = arptab;
    while(current->next != NULL){
        prev = current;
        current = current->next;
        if(memcmp(ip, current->ipaddr, IP_ADDR_LEN)){
            prev->next = current->next;
            free((void *)current);
            break;
        }
    }
    signal(arpdelete_sem);
    return OK;
}

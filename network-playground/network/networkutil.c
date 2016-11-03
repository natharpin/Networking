#include <xinu.h>

int firstOcc(char c, char *str, int index){
    int i;
    for(i = index + 1; i < strlen(str); i++){
       if(c == str[i])
           return i;
    }
    return -1;
}

bool isValidIpAddress(char *ipaddr){
    int i;
    int period = 0;
    for(i = 0; i < 3; i++){
        if(firstOcc('.', ipaddr, period) != period && firstOcc('.', ipaddr, period) != -1)
            period = firstOcc('.', ipaddr, period);
        else
            return 0;
    }
    return 1;
}

int ether_swap(void *buf){
    struct ethergram *gram = (struct ethergram *)buf;
    uchar temp[ETH_ADDR_LEN];

    if (SYSERR == getmac(temp)){
        return SYSERR;
    }
    colon2mac(nvramGet("et0macaddr\0"), temp);

    memcpy(gram->dst, gram->src, ETH_ADDR_LEN);
    memcpy(gram->src, temp, ETH_ADDR_LEN);

    return OK;
}

int ipv4_swap(void *buf, int length){
    struct ipv4gram *gram = (struct ipv4gram *)buf;
    char temp[IP_ADDR_LEN];

    gram->chksum = htons(0x0000);

    memcpy(temp, gram->dst, IP_ADDR_LEN);
    memcpy(gram->dst, gram->src, IP_ADDR_LEN);
    memcpy(gram->src, temp, IP_ADDR_LEN);

    gram->chksum = checksum(buf, length);

    return OK;
}

int getmac(void *buf){
    char *mac;

    mac = nvramGet("et0macaddr\0");

    if (NULL == mac){
        return SYSERR;
    }

    colon2mac(mac, (uchar *)buf);

    return OK;
}

int getip(void *buf){
    char *ip;

    ip = nvramGet("lan_ipaddr\0");

    if (NULL == ip){
        return SYSERR;
    }

    dot2ip(ip, (uchar *)buf);

    return OK;
}


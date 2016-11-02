#include <xinu.h>

int firstOcc(char c, char *str, int index){
    int i;
    for(i = index + 1; i < strlen(str); i++){
       if(c == str[i])
           return i;
    }
    return -1;
}

bool(isValidIpAddress(char *ipaddr)){
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


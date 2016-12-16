#include <xinu.h>

void addFrag(int seq, int length, void *payload){

    struct fragment *curr = fragHead;
    struct fragment *prev = curr;
    
    while((curr = curr->next) != NULL){
        if(curr->seq > seq){
            struct fragment *newFrag = (struct fragment *)malloc(sizeof(struct fragment));
            newFrag->seq = seq;
            newFrag->length = length;
            prev->next = newFrag;
            newFrag->next = curr;
            memcpy(newFrag->buffer, payload, length);
            return;
        }
        prev = curr;
    }
    
    struct fragment *newFrag = (struct fragment *)malloc(sizeof(struct fragment));
    prev->next = newFrag;
    newFrag->seq = seq;
    newFrag->length = length;
    memcpy(newFrag->buffer, payload, length);
}

void getFrag(uchar *buff){
    void *offset = buff;
    
    struct fragment *curr = fragHead;
    
    while((curr = curr->next) != NULL){
        memcpy(offset, curr->buffer, curr->length);
        offset += curr->length;
    }
}

void clearFrag(){
    fragHead = (struct fragment *)malloc(sizeof(struct fragment));
    fragHead->next = NULL;
    fragHead->seq = -1;
}

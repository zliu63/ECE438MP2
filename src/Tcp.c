//
//  TCP.c
//  
//
//  Created by Liuyi Shi on 10/20/17.
//

#include "TCP.h"

void init_Seg(TCP_Seg * seg){
    seg->SYN = 0;
    seg->FIN = 0;
    seg->SEQ = 0;
    seg->ACK = 0;
    seg->CHK = 0;
    seg->RWND = 0;
    seg->LEN = 0;
    for(int i = 0; i < MSS; i++)
        seg->data[i] = 0;
}

void make_SYN_Seg(TCP_Seg * seg, int seq){
    
    init_Seg(seg);
    
    seg->SYN = 1;
    seg->SEQ = seq;
    seg->CHK = generateCheckSum( *seg );
    
}

unsigned char generateCheckSum(TCP_Seg seg){
    seg.CHK = 0;
    size_t sz = sizeof(seg);
    
    unsigned char *p = (unsigned char *) &seg;
    unsigned char CHK = 0;
    
    while (sz-- != 0)
        CHK -= *p++;
    
    return CHK;
}

int IsCorrupted(TCP_Seg seg){
    unsigned char CHK = seg.CHK;
    
    seg.CHK = 0;
    
    size_t sz = sizeof(seg);
    
    unsigned char *p = (unsigned char *) &seg;
    
    while (sz-- != 0)
        CHK += *p++;
    
    if(CHK == 0)
        return 0;
    else
        return 1;
    
}

void make_Seg(TCP_Seg *seg, int seq, int ack, int len, char *data){
    init_Seg(seg);
    
    seg->SEQ = seq;
    seg->ACK = ack;
    seg->LEN = len;
    memcpy(seg->data, data, len);
    
    seg->CHK = generateCheckSum( *seg );
}

//
//  TCP.H
//  
//
//  Created by Liuyi Shi on 10/20/17.
//

#ifndef segment_h
#define segment_h

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
#define MAX_SEQ 1024
#define WINDOW_SIZE MAX_SEQ/10
#define MSS 100


//Status for segments
enum Seg_Status_t{NOTSEND, UNACKED, ACKED, NOTEXIST};
typedef enum Seg_Status_t Seg_Status;

//define TCP segment
struct Segment{
    int SYN;
    int FIN;
    int SEQ;
    int ACK;
    unsigned char CHK;
    int RWND;
    int LEN;
    char data[MSS];
};

typedef struct Segment TCP_Seg;

void make_SYN_Seg(TCP_Seg * seg, int seq);

//generate CheckSum and return
unsigned char generateCheckSum(TCP_Seg seg);

//return 1 if corrupted, 0 if not
int IsCorrupted(TCP_Seg seg);

//make a segment
void make_Seg(TCP_Seg *seg, int seq, int ack, int len, char *data);

#endif

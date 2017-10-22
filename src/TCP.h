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

#define MAX_SEQ 1024
#define WINDOW_SIZE MAX_SEQ/50
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

void make_SYN_Seg(TCP_Seg * seg, int seq, int ack);
void make_FIN_Seg(TCP_Seg * seg, int seq, int ack);

//generate CheckSum and return
unsigned char generateCheckSum(TCP_Seg seg);

//return 1 if corrupted, 0 if not
int IsCorrupted(TCP_Seg seg);

//make a segment
void make_Seg(TCP_Seg *seg, int seq, int ack, int len, char *data);

//return seq + addend, but will still be in range of 0 to MAX_SEQ-1. Addend can also be negetive
//for example: 1022+5 return 3, 5-3 return 1022
int SeqAdd( int seq, int addend );

//compare seq0 and seq1, return how much seq0 is bigger than seq1. Return negetive value if seq0 is smaller than seq1.
// for example:
//seq0 = 10 seq1 = 20, return -10
//seq0 = 20 seq1 = 10, return 10
//seq0 = 1020 seq1 = 5, return -8
//seq0 = 5 seq1 = 1020, return 8
int SeqCompare( int seq0, int seq1);


#endif

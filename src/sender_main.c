/* 
 * File:   sender_main.c
 * Author: 
 *
 * Created on 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include "TCP.h"
#include "List.h"
#include <sys/unistd.h>
#include <sys/fcntl.h>
//linked list to act as buffer
Node *list;

//the base sequence number of sending window
int base;
//the sequence number of next unsended packet
int nextSeq;

//total number of segment in buffer(list)
int SegNum;
//number of sent but unacked segments in buffer
int SegUnAcked;
//number of segments in buffer not send yet
int SegNotSend;

//save the time that the timer is started
time_t TimerStartAt;
//control if timer is running
int IsTimerOn;


struct sockaddr_in si_other;
socklen_t s;
int slen;

void diep(char *s) {
    perror(s);
    exit(1);
}

//Send next UnSended segment in buffer.
void SendSegment(){
    TCP_Seg seg;
    if( SeqCompare( nextSeq, SeqAdd( base, WINDOW_SIZE) ) < 0 ){
        seg = GetSeg(list, nextSeq);
        sendto(s, &seg, sizeof(TCP_Seg), 0, (struct sockaddr*) &si_other, sizeof (si_other));
        SetNodeStatus(list, nextSeq, UNACKED);
        SegUnAcked = GetStatusNumber(list, UNACKED);
        SegNotSend = GetStatusNumber(list, NOTSEND);
        if(base == nextSeq){
            TimerStartAt = clock();
            IsTimerOn = 1;
        }
        SeqAdd(nextSeq,1);
    }
}

//Function dealing with received ACK
void ReceiveACK(TCP_Seg seg){
    if( SeqCompare(seg.ACK,base) >= 0 ){
        for(int i = 0; SeqCompare( SeqAdd(base,i), ACK ) < 0 ; i++){
            SetNodeStatus(list, SeqAdd(base,i), ACKED);
            RemoveNode( &list, SeqAdd(base,i) );
            SegUnAcked = GetStatusNumber(list, UNACKED);
            SegNum = GetNumber(list);
            SeqAdd(base,1);
        }
        if(SegUnAcked != 0){
            TimerStartAt = clock();
            IsTimerOn = 1;
        }
        else
            IsTimerOn = 0;
    }
}

//Function dealing with timeout
void TimeOut(){
    TCP_Seg seg;
    seg = GetSeg(list, base);
    TimerStartAt = clock();
    IsTimerOn = 1;
}

//Add a segment to buffer
void AddSegToBuffer(TCP_Seg seg){
    AddNode(seg, NOTSEND);
    SegNotSend = GetStatusNumber(list, NOTSEND);
    SegNum = GetNumber(list);
}


void reliablyTransfer(char* hostname, unsigned short int hostUDPport, char* filename, unsigned long long int bytesToTransfer) {
//    Node *head;
//    TCP_Seg seg;
//    seg.SEQ = 1;
//    AddNode(&head, seg, UNACKED);
//    seg.SEQ = 2;
//    AddNode(&head, seg, NOTSEND);
//    seg.SEQ = 3;
//    AddNode(&head, seg, UNACKED);
//    seg.SEQ = 4;
//    AddNode(&head, seg, NOTSEND);
//    seg.SEQ = 5;
//    AddNode(&head, seg, NOTSEND);
//    seg.SEQ = 6;
//    AddNode(&head, seg, UNACKED);
//    seg.SEQ = 7;
//    AddNode(&head, seg, UNACKED);
//    printf("%d\n", GetStatus(head,5));
//    printf("total:%d UNACKED:%d UNSEND:%d ACKED:%d\n", GetNumber(head), GetStatusNumber(head,UNACKED), GetStatusNumber(head,NOTSEND), GetStatusNumber(head,ACKED) );
//    printf("setreturn:%d\n", SetNodeStatus(head,5,ACKED));
//    printf("setreturn:%d\n", SetNodeStatus(head,8,ACKED));
//    printf("%d\n", GetStatus(head,5));
//    printf("total:%d UNACKED:%d UNSEND:%d ACKED:%d\n", GetNumber(head), GetStatusNumber(head,UNACKED), GetStatusNumber(head,NOTSEND), GetStatusNumber(head,ACKED) );
//    printf("REMOVERETURN:%d\n",   RemoveNode( &head, 5));
//    printf("REMOVERETURN:%d\n",   RemoveNode( &head, 2));
//    printf("REMOVERETURN:%d\n",   RemoveNode( &head, 6));
//    printf("REMOVERETURN:%d\n",   RemoveNode( &head, 8));
//    printf("total:%d UNACKED:%d UNSEND:%d ACKED:%d\n", GetNumber(head), GetStatusNumber(head,UNACKED), GetStatusNumber(head,NOTSEND), GetStatusNumber(head,ACKED) );

    
    
    
    //printf("%d\n",SeqCompare( 51, SeqAdd( 1020, 50) ));

	/* Determine how many bytes to transfer */

    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        diep("socket");
    
    //set recvfrom to be unblock, or we will stuck there.
    fcntl(s, F_SETFL, O_NONBLOCK);

    memset((char *) &si_other, 0, sizeof (si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(hostUDPport);
    if (inet_aton(hostname, &si_other.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
    //Hand Shake process
    int InitSeq = rand() % MAX_SIZE;
    base = InitSeq;
    nextSeq = InitSeq;

//    while(1){
//        while( SegNum < WINDOW_SIZE){
//            if(file not end){
//                
//            }
//        }
//        
//        if(1){
//            if( SegNotSend != 0){
//                SendSegment();
//            }
//        }
//        
//        if( (recvfrom(s, &s_rec, sizeof(TCP_Seg), 0, (struct sockaddr*) &si_other, &slen)) != -1 ){
//            ReceiveACK
//        }
//        
//        if( IsTimerOn == 1 ){
//            if( (clock() - TimerStartAt) >= TimeOut){
//                TimeOut();
//            }
//        }
//        
//        
//    }
    
    close(s);
    return;

}

/*
 * 
 */
int main(int argc, char** argv) {

    unsigned short int udpPort;
    unsigned long long int numBytes;

//    if (argc != 5) {
//        fprintf(stderr, "usage: %s receiver_hostname receiver_port filename_to_xfer bytes_to_xfer\n\n", argv[0]);
//        exit(1);
//    }
    udpPort = (unsigned short int) atoi(argv[2]);
    numBytes = atoll(argv[4]);



    reliablyTransfer(argv[1], udpPort, argv[3], numBytes);


    return (EXIT_SUCCESS);
}



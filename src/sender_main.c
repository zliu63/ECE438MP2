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

int base;
int nextSeq;

int segNum;
int SegUnAcked;
int SegNotSend;

struct sockaddr_in si_other;
socklen_t s;
int slen;
time_t timer;

void diep(char *s) {
    perror(s);
    exit(1);
}

void reliablyTransfer(char* hostname, unsigned short int hostUDPport, char* filename, unsigned long long int bytesToTransfer) {
    int numbytes;
    char buf[BUFFER_SIZE] = "this is a test from send!";
	
    TCP_Seg s_snd;
    TCP_Seg s_rec;
    
    
//    /* TEST CODE */
//    TCP_Seg seg;
//    Node *list;
//    Node node;
//    Seg_Status status;
//
//    printf("Begin Testing\n");
//
//    make_SYN_Seg(&seg, 12);
//    AddNode( &list, seg, NOTSEND);
//
//    make_Seg(&seg,15,20,strlen("THIS IS A TEST") + 1, "THIS IS A TEST");
//    AddNode( &list, seg, NOTSEND);
//
//    make_Seg(&seg,18,20,strlen("THIS IS A NEW TEST") + 1, "THIS IS A NEW TEST");
//    AddNode( &list, seg, NOTSEND);
//
//    printf("Num Nodes: %d\n",GetNumber(list));
//
//    node = GetNode(list, 15);
//    seg = node.seg;
//    status = node.status;
//
//    puts(seg.data);
//    printf("Status: %d\n",status);
//
//    printf("IsCorrupted: %d\n",IsCorrupted(seg));
//    /* TEST CODE */
    
    
    
    //Open the file
   // FILE *fp;
   // fp = fopen(filename, "rb");
   // if (fp == NULL) {
   //     printf("Could not open file to send.");
   //     exit(1);
   // }

	/* Determine how many bytes to transfer */

    slen = sizeof (si_other);

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
    
//    /* Send data and receive acknowledgements on s*/
//
//    timer = clock();
//    
//    memcpy(s_snd.data, "This is a data!", strlen("This is a data!") + 1);
//    
//    sendto(s, &s_snd, sizeof(TCP_Seg), 0, (struct sockaddr*) &si_other, slen);
//    
//    while( (recvfrom(s, &s_rec, sizeof(TCP_Seg), 0, (struct sockaddr*) &si_other, &slen)) == -1){
//        if( (int)( clock() - timer ) > 1000 ){
//            sendto(s, &s_snd, sizeof(TCP_Seg), 0, (struct sockaddr*) &si_other, slen);
//            timer = clock();
//        }
//    };
//    
//    if(s_rec.ACK == 1){
//        puts("ACK received.\n");
//        printf("Total time: %d\n",(int)( clock() - timer ));
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



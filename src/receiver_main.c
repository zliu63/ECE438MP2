 /* 
 * File:   receiver_main.c
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
#include "TCP.h"
#include "List.h"
#include <sys/unistd.h>
#include <sys/fcntl.h>

//the seq of next packet that is waitting for
int expectedSeq;

struct sockaddr_in si_me, si_other;
socklen_t s;
int slen;

void diep(char *s) {
    perror(s);
    exit(1);
}

//write the data from a segment to file
void write_to_file(){
    
}

void reliablyReceive(unsigned short int myUDPport, char* destinationFile) {
	
    slen = sizeof (si_other);

    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        diep("socket");

    memset((char *) &si_me, 0, sizeof (si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(myUDPport);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (bind(s, (struct sockaddr*) &si_me, sizeof (si_me)) == -1)
        diep("bind");
    
    fcntl(s, F_SETFL, O_NONBLOCK);
    
    TCP_Seg seg_r;
    TCP_Seg seg_s;
    
    //Hankshake process, wait until a packet is received.
    while(1){
        if( (recvfrom(s, &seg_r, sizeof(TCP_Seg), 0, (struct sockaddr*) &si_other, &slen)) != -1){
            //if it is a SYN packet and is not corrupted, send back a SYN packet with ACK = SEQ + 1
            if( ( seg_r.SYN == 1 ) && ( IsCorrupted(seg_r) == 0 ) ){
                expectedSeq = SeqAdd(seg_r.SEQ, 1);
                make_SYN_Seg( &seg_s, 0, expectedSeq );
                sendto(s, &seg_s, sizeof(TCP_Seg) , 0, (struct sockaddr*) &si_other, slen);
                puts("Connected successfully!");
                break;
            }
            else{
                puts("Error setting up connection!");
            }
        };
    }
    
    while(1){
        //if received a packet
        if( (recvfrom(s, &seg_r, sizeof(TCP_Seg), 0, (struct sockaddr*) &si_other, &slen)) != -1){
            //if is expected number and not corrupted, write the data to file and add exceptedSeq by 1
            //else do nothing
            if( ( seg_r.SEQ == expectedSeq ) && ( IsCorrupted(seg_r) == 0 ) ){
                expectedSeq = SeqAdd(expectedSeq, 1);
                if(seg_r.FIN != 1){
                    write_to_file();//to be implement;
                }
                else{
                    make_FIN_Seg( &seg_s, 0, expectedSeq );
                    sendto(s, &seg_s, sizeof(TCP_Seg) , 0, (struct sockaddr*) &si_other, slen);
                    puts("Finish!");
                    break;
                }
            }
            //send ACK segment back
            make_Seg(&seg_s, 0, expectedSeq, 0, NULL);
            sendto(s, &seg_s, sizeof(TCP_Seg) , 0, (struct sockaddr*) &si_other, slen);
        }
    }

    close(s);
    
	//printf("%s received.", destinationFile);
    
    return;
}

/*
 * 
 */
int main(int argc, char** argv) {

    unsigned short int udpPort;

//    if (argc != 3) {
//        fprintf(stderr, "usage: %s UDP_port filename_to_write\n\n", argv[0]);
//        exit(1);
//    }

    udpPort = (unsigned short int) atoi(argv[1]);

    reliablyReceive(udpPort, argv[2]);
}


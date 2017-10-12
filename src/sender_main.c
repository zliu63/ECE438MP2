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

#define BUFFER_SIZE 1024
#define SEQUENCE_LIMIT 4096
#define MSS 100

struct sockaddr_in si_other;
socklen_t s;
int slen;
time_t timer;

//define TCP segment
typedef struct Segment{
	int SYN;
	int FIN;
	int SEQ;
	int ACK;
	int CHK;
	int RWND;
    int LEN;
	char data[MSS];
} TCP_Seg;

//init TCP Segment
TCP_Seg segment_init(){
	TCP_Seg seg;
	seg.SYN = 0;
	seg.FIN = 0;
	seg.SEQ = 0;
	seg.ACK = 0;
	seg.CHK = 0;
	seg.RWND = 0;
    seg.LEN = 0;
	for(int i = 0; i < MSS; ++i)
		seg.data[i] = 0;
	return seg;
}

void diep(char *s) {
    perror(s);
    exit(1);
}


void reliablyTransfer(char* hostname, unsigned short int hostUDPport, char* filename, unsigned long long int bytesToTransfer) {
    int numbytes;
    char buf[BUFFER_SIZE] = "this is a test from send!";
	
    TCP_Seg s_snd = segment_init();
    TCP_Seg s_rec;
		
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
    
    fcntl(s, F_SETFL, O_NONBLOCK);

    memset((char *) &si_other, 0, sizeof (si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(hostUDPport);
    if (inet_aton(hostname, &si_other.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
    
	/* Send data and receive acknowledgements on s*/
    printf("Send Connection Request\n");
    timer = clock();
    
    s_snd.SYN = 1;
    s_snd.SEQ = 10;
    
    while(s_rec.SYN == 0){
        sendto(s, &s_snd, sizeof(TCP_Seg), 0, (struct sockaddr*) &si_other, slen);
        recvfrom(s, &s_rec, sizeof(TCP_Seg), 0, (struct sockaddr*) &si_other, &slen);
    }
    
    printf("Connected!\n");
    
    while(s_rec.FIN == 0){
        s_snd.SEQ = s_rec.ACK;
        s_snd.ACK = s_rec.SEQ + 1;
        sendto(s, &s_snd, sizeof(TCP_Seg), 0, (struct sockaddr*) &si_other, slen);
        recvfrom(s, &s_rec, sizeof(TCP_Seg), 0, (struct sockaddr*) &si_other, &slen);
        printf("%d %d\n", s_rec.SEQ, s_rec.ACK);
    }

    
    sendto(s, &s_snd, sizeof(TCP_Seg) , 0, (struct sockaddr*) &si_other, slen);
    
    while(s_rec.ACK <= strlen(buf)){
        sendto(s, &s_snd, sizeof(TCP_Seg), 0, (struct sockaddr*) &si_other, slen);
        //printf("SND_SEQ = %d\n", s_snd.SEQ);
        recvfrom(s, &s_rec, sizeof(TCP_Seg), 0, (struct sockaddr*) &si_other, &slen);
       // printf("REC_SEQ = %d\n", s_rec.SEQ);
        printf("%c\n", s_rec.SYN);
        s_snd.SEQ = s_rec.ACK;
        s_snd.SYN = buf[s_snd.SEQ];
        s_snd.ACK = s_rec.SEQ + 1;
    }
    printf("Closing the socket\n");
    close(s);
    return;

}

/*
 * 
 */
int main(int argc, char** argv) {

    unsigned short int udpPort;
    unsigned long long int numBytes;

    if (argc != 5) {
        fprintf(stderr, "usage: %s receiver_hostname receiver_port filename_to_xfer bytes_to_xfer\n\n", argv[0]);
        exit(1);
    }
    udpPort = (unsigned short int) atoi(argv[2]);
    numBytes = atoll(argv[4]);



    reliablyTransfer(argv[1], udpPort, argv[3], numBytes);


    return (EXIT_SUCCESS);
}



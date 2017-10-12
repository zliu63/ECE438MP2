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

#define BUFFER_SIZE 1024
#define SEQUENCE_LIMIT 4096
#define MSS 100

struct sockaddr_in si_me, si_other;
socklen_t s;
int slen;

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
	for(int i = 0; i < MSS; ++i)
		seg.data[i] = 0;
	return seg;
}

void diep(char *s) {
    perror(s);
    exit(1);
}

void reliablyReceive(unsigned short int myUDPport, char* destinationFile) {
    int numbytes;
    char buf[BUFFER_SIZE] = "This is a test from rec!";
	
	TCP_Seg s_rec= segment_init();
	TCP_Seg s_snd = segment_init();
	
    slen = sizeof (si_other);


    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        diep("socket");

    memset((char *) &si_me, 0, sizeof (si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(myUDPport);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    printf("Now binding\n");
    if (bind(s, (struct sockaddr*) &si_me, sizeof (si_me)) == -1)
        diep("bind");
    
    fcntl(s, F_SETFL, O_NONBLOCK);

    while(s_rec.SYN == 0){
        recvfrom(s, &s_rec, sizeof(TCP_Seg), 0, (struct sockaddr*) &si_other, &slen);
        printf("Waitting for connection!\n");
    }
    
    printf("Received Connection Request From sender!\n");
    s_snd.SYN = 1;
    s_snd.SEQ = 25;
    s_snd.ACK = s_rec.SEQ + 1;
    sendto(s, &s_snd, sizeof(TCP_Seg) , 0, (struct sockaddr*) &si_other, slen);
    
    while(s_rec.FIN == 0){
        recvfrom(s, &s_rec, sizeof(TCP_Seg), 0, (struct sockaddr*) &si_other, &slen);
        printf("%d %d\n", s_rec.SEQ, s_rec.ACK);
        s_snd.SEQ = s_rec.ACK;
        s_snd.ACK = s_rec.SEQ + 1;
        sendto(s, &s_snd, sizeof(TCP_Seg), 0, (struct sockaddr*) &si_other, slen);
    }
    
	/* Now receive data and send acknowledgements */
	while(s_rec.ACK <= strlen(buf)){
	    recvfrom(s, &s_rec, sizeof(TCP_Seg), 0, (struct sockaddr*) &si_other, &slen);
		printf("%c\n", s_rec.SYN);
		//printf("REC_SEQ = %d\n", s_rec.SEQ);
        s_snd.SEQ = s_rec.ACK;
		s_snd.SYN = buf[s_snd.SEQ];
		s_snd.ACK = s_rec.SEQ + 1;

		sendto(s, &s_snd, sizeof(TCP_Seg) , 0, (struct sockaddr*) &si_other, slen);
        //printf("SND_SEQ = %d\n", s_snd.SEQ);
	}
  
    close(s);
	printf("%s received.", destinationFile);
    return;
}

/*
 * 
 */
int main(int argc, char** argv) {

    unsigned short int udpPort;

    if (argc != 3) {
        fprintf(stderr, "usage: %s UDP_port filename_to_write\n\n", argv[0]);
        exit(1);
    }

    udpPort = (unsigned short int) atoi(argv[1]);

    reliablyReceive(udpPort, argv[2]);
}


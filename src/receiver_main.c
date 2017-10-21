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

struct sockaddr_in si_me, si_other;
socklen_t s;
int slen;

void diep(char *s) {
    perror(s);
    exit(1);
}

void reliablyReceive(unsigned short int myUDPport, char* destinationFile) {
    int numbytes;
    char buf[BUFFER_SIZE] = "This is a test from rec!";
	
    TCP_Seg s_rec;
    TCP_Seg s_snd;
	
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
    
    puts("recv");
    
    while(1){
        if( (recvfrom(s, &s_rec, sizeof(TCP_Seg), 0, (struct sockaddr*) &si_other, &slen)) != -1){
            puts(s_rec.data);
            
            s_snd.ACK = 1;
            sendto(s, &s_snd, sizeof(TCP_Seg) , 0, (struct sockaddr*) &si_other, slen);
        };
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


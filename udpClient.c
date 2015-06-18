
#include <netdb.h>

#include <stdio.h>

#include <string.h>

#include <time.h>

#include <unistd.h>

#include <netinet/in.h>

#include <sys/socket.h>

#include <sys/types.h>

#define BUFFER_SIZE 64000

void sendBytes(const char *port, const char 

*hostname, const int BYTE_SIZE, const int 

LOOP_AMOUNT);

int main(int argc, char *argv[]) {

    if (argc < 3) {

        printf("Error, port/ip address argument is missing\n");

        return 0;

    }

    

     // a good port is 8090

    sendBytes(argv[1], argv[2], 1, 1000);

    sendBytes(argv[1], argv[2], 1024, 100);

    sendBytes(argv[1], argv[2], 1024 * 4, 100);

    sendBytes(argv[1], argv[2], 1024 * 8, 100);

    sendBytes(argv[1], argv[2], 1024 * 16, 100);

    sendBytes(argv[1], argv[2], 1024 * 32, 100);

    sendBytes(argv[1], argv[2], 64000, 100);

    

    return 0;

}

void sendBytes(const char *port, const char *hostname, const int BYTE_SIZE,const int LOOP_AMOUNT) {

    clock_t startClock, endClock;

    struct sockaddr_in server, client;

    int i, sock, serverLength, result, lostPackets;

    double elapsedTime, totalRTT;

    socklen_t clientLength;

    struct hostent *host; 

    char outBuffer[BYTE_SIZE], 

inBuffer[BUFFER_SIZE];

    // socket - a connection endpoint

    sock = socket(PF_INET, SOCK_DGRAM,0); 
// SOCK_DGRAM is for UDP,SOCK_STREAM is for TCP

    if(sock < 0) {

        printf("Error on socket\n");

        return;

    }

    // gethostbyname - gets the server's address

    host = gethostbyname(hostname); 
//"localhost" refers to ourself

    if(host == 0) {

        printf("Error on gethostbyname\n");

        return;

    }

    bcopy((char *)host->h_addr, (char 

*)&server.sin_addr, host->h_length);

    // setup server

    serverLength = sizeof(server);

    bzero(&server, serverLength);

    server.sin_family = PF_INET;

    server.sin_port = htons(atoi(port));

    server.sin_addr.s_addr = INADDR_ANY;

    // setup buffers

    for(i = 0; i < BYTE_SIZE - 1; i++) {

        outBuffer[i] = 'a';

    }

    outBuffer[BYTE_SIZE - 1] = '\0';

    

    bzero(inBuffer, BUFFER_SIZE);

    

    // send LOOP_AMOUNT messages of BYTE_SIZE

    lostPackets = 0;

    totalRTT = 0;

    clientLength = sizeof(client);

    for(i = 0; i < LOOP_AMOUNT; i++) {

        // setup startClock

        startClock = clock();

        

        // sendto - send a message

        result = sendto(sock, outBuffer, 

BYTE_SIZE, 0, (struct sockaddr *)&server, 

serverLength);

        if(result < 0) {

            printf("Error on sendto\n");

            lostPackets++;

        }

        else {

            // recvfrom - receive acknowledgement from server

            result = recvfrom(sock, inBuffer, 

BUFFER_SIZE - 1, 0, (struct sockaddr

*)&client, &clientLength);

            if(result < 0) {

                printf("Error on recvfrom\n");

            }

            

            // calculate totalRTT

            endClock = clock();

            elapsedTime = (double) (endClock -startClock) / CLOCKS_PER_SEC;

            totalRTT += elapsedTime;

            //printf("%2d: %fs, %fs\n", i,elapsedTime, totalRTT);

        }

    }

    // close - end the connection

    close(sock);

    

    // calculate throughput, average RTT, etc

    printf("Stats on sending %d bytes %d times\n", BYTE_SIZE, LOOP_AMOUNT);

    double delay = totalRTT / (LOOP_AMOUNT + 0.0f);

    printf("Average RTT (delay): %fs\n",delay);

    double delayWithoutLostPackets = totalRTT / (LOOP_AMOUNT - lostPackets + 0.0f);

    printf("Number of lost packets: %ds\n",lostPackets);

    printf("Average delay without lost packets:%fs\n", delayWithoutLostPackets);

    double throughput = BYTE_SIZE / delayWithoutLostPackets; 

    printf("Average throughput: %fBps\n",throughput);

    printf("\n");

    

    return;

}

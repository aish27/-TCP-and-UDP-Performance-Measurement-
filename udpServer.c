#include <stdio.h>

#include <string.h>

#include <unistd.h>

#include <netinet/in.h>

#include <sys/socket.h>

#include <sys/types.h>

#define BUFFER_SIZE 64000

int main(int argc, char *argv[]) {

    if (argc < 2) {

         printf("Error, port argument is missing\n");

        return 0;

    }

    

    struct sockaddr_in server, client;

    int sock, serverLength, result;

    socklen_t clientLength;

    char buffer[BUFFER_SIZE];

    // socket - a connection endpoint

    sock = socket(PF_INET, SOCK_DGRAM, 0); // SOCK_DGRAM is for UDP, SOCK_STREAM is for TCP

    if(sock < 0) {

        printf("Error on socket\n");

        return 0;

    }

    // setup server

    serverLength = sizeof(server);

    bzero(&server, serverLength);

    server.sin_family = PF_INET;

    server.sin_port = htons(atoi(argv[1])); 
// a good port is 8090

    server.sin_addr.s_addr = INADDR_ANY;

    // bind - attaching to an IP and Port

    if(bind(sock, (struct sockaddr *) &server, 

sizeof(server)) < 0) {

        printf("Error on bind\n");

        return 0; 

    }

    // wait for clients to connect

    printf("Server is waiting...");

    clientLength = sizeof(client);

    while(1) {

        // recvfrom - receive message from client

        result = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client, &clientLength);

        if(result < 0) {

            printf("Error on recvfrom\n");

        }

        else {

            printf("Received %d bytes\n", strlen(buffer) + 1);

            //printf("Received %d bytes: %s\n",strlen(buffer) + 1, buffer); // slowdown for concurrent connections

            // sendto - send an acknowledgement

            result = sendto(sock, buffer,strlen(buffer), 0,(struct sockaddr *)&client, clientLength);

            if(result  < 0) {

                printf("Error on sendto\n");

            }      

        }

    }

    // close - end the connection

    close(sock);

    return 0;

}

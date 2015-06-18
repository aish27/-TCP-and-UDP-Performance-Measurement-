#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include <time.h>

#define MSize 64000



void sendBytes(const char *port, const char *hostname, const int BYTE_SIZE, const int LOOP_AMOUNT);

int main(int argc , char *argv[])
{

    if (argc < 3) {
        printf("Error, port/ip address argument is missing\n");
        return 0;
    }
    
  
    sendBytes(argv[1], argv[2], 1, 1000);
    sendBytes(argv[1], argv[2], 1024, 1000);
    sendBytes(argv[1], argv[2], 1024 * 4, 1000);
    sendBytes(argv[1], argv[2], 1024 * 8, 1000);
    sendBytes(argv[1], argv[2], 1024 * 16, 1000);
    sendBytes(argv[1], argv[2], 1024 * 32, 1000);
    sendBytes(argv[1], argv[2], 64000, 1000);
   

    return 0;
}

void sendBytes(const char *port, const char *hostname, const int BYTE_SIZE, const int LOOP_AMOUNT) {

     
    clock_t startClock, endClock;
    double elapsed=0.0f ;
    double totalRTT=0.0f;
    
    int sock;
    struct sockaddr_in server;
    char  server_reply[MSize];
    char  Msg_size[BYTE_SIZE];
   
    
    int x=0;
    for(x = 0; x < BYTE_SIZE; x++) {
        Msg_size[x] = 'a';
    }

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);  
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    
    //get server information
    server.sin_addr.s_addr = inet_addr(hostname);
    server.sin_family = AF_INET;
    server.sin_port = htons( atoi(port) );
 
    //Connect to server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return;
    }
 
    puts("Connected\n");

    //keep communicating with server
    int y=0;    
    for(y=0;y<LOOP_AMOUNT;y++){
        
         startClock = clock();

        //Send some data     
        if( send(sock , Msg_size , BYTE_SIZE, 0) < 0)
        {
            puts("Send failed");
            return;
        }       
       
        //Receive a reply from the server
        int len=0;       
        if(len = recv(sock , server_reply , sizeof(server_reply) , 0) < 0)
        {
            puts("recv failed");
        } 
  
        endClock = clock();
        double x=(double)CLOCKS_PER_SEC;
        elapsed =  ((double)endClock - (double)startClock)/x;
        totalRTT = totalRTT+elapsed;
        
    }
     
    printf("\n");
    printf("Stats on sending %d bytes %d times\n", BYTE_SIZE, LOOP_AMOUNT);
    double delay = totalRTT / (LOOP_AMOUNT + 0.0f);
    printf("Average RTT (delay): %fms\n", delay*1000);
    double throughput = BYTE_SIZE*8 / delay;
    printf("Average throughput: %fbps\n", throughput);
    printf("\n");
    
    
    close(sock);
    
 
    return ;
  
}


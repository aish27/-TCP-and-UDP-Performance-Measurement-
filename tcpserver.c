#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#define MSize 65000

int main(int argc, char *argv[])
{
    
    if (argc < 2) {
         printf("Error, port argument is missing\n");
        return 0;
    }
   
   
    int server = 0, client = 0;
    struct sockaddr_in server_addr; 
    struct sockaddr_in client_addr; 
    int len,sin_size ;
    char buf[MSize];

    //create TCP socket
    server = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1])); 
    sin_size = sizeof(struct sockaddr_in);
    
    //bind socket
    bind(server, (struct sockaddr*)&server_addr, sizeof(server_addr)); 
    
    //listening for connection
    listen(server, 65000); 
    puts("accepting connection\n");
    

    while(1){
        
           //accept connection
           client = accept(server, (struct sockaddr*)& client_addr, &sin_size); 
            
           if (client < 0) {
             perror("error");
             exit(1);
           }
          
          //keep communicating with client 
          while(len=recv(client, buf,  sizeof(buf) , 0)>0){ 
            //buf[len]='\0';
            //printf("%s\n", buf);  
            char *msg = "Hello!"; 
            int size;
            size = strlen(msg);
            send(client, msg, sizeof(msg), 0);
            //c++;
            //printf("%d\n", c);  
            //bzero(buf, MSize);
         }
           
            close(client);
    }
}



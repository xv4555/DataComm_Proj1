#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

char* deblank(char* input)                                         
{
    int i,j;
    char *output=input;
    for (i = 0, j = 0; i<strlen(input); i++,j++)          
    {
        if (input[i]!='\n')                           
            output[j]=input[i];                     
        else
            j--;                                     
    }
    output[j]=0;
    return output;
}

int main(int argc, char** argv){
    struct Node 
    { 
        char *name;
        char *password;
        struct Node *next; 
    }; 

    if(argc != 3)
    {
        printf("Usage: ./351ChatServer <port> <num_clients>\n");
        return 0;
    }

    int sockfd, ret, newSocket;
    int port = atoi(argv[1]);   //Check for Error
    int num_clients = atoi(argv[2]);   // Check for Errors

    if(num_clients <= 10)
    {
        printf("Usage: num_clients must be larger than 10\n");
        return 0;
    }

    struct sockaddr_in serverAddr;
    struct sockaddr_in newAddr;
    socklen_t addr_size;
    char buffer[1024];
    pid_t childpid;

    int opt = 1;
    fd_set readfds;
    int client_socket[30], activity, i , valread , sd, max_sd, addrlen, k;

    for (i = 0; i < num_clients; i++) 
    {
        client_socket[i] = 0;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        printf("Error in connection.\n");
        exit(1);
    }

    if( setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    printf("Server Socket is created.\n");
    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    
    if(ret < 0){
        printf("Error in binding.\n");
        exit(1);
    }

    printf("Bind to port %d\n", port);

    // if(listen(sockfd, num_clients) == 0){
    //  printf("Listening...\n");
    // }else{
    //  printf("Error in binding.\n");
    // }

    if (listen(sockfd, num_clients) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    addrlen = sizeof(serverAddr);
    puts("Waiting for connections ...");

    while(1){

        //clear the socket set
        FD_ZERO(&readfds);
  
        //add master socket to set
        FD_SET(sockfd, &readfds);
        max_sd = sockfd;

        //add child sockets to set
        for ( i = 0 ; i < num_clients ; i++) 
        {
            //socket descriptor
            sd = client_socket[i];
             
            //if valid socket descriptor then add to read list
            if(sd > 0)
                FD_SET( sd , &readfds);
             
            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
                max_sd = sd;
        }
  
        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
    
        if ((activity < 0) && (errno!=EINTR)) 
        {
            printf("select error");
        }

        //If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(sockfd, &readfds)) 
        {
            if ((newSocket = accept(sockfd, (struct sockaddr *)&serverAddr, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
          
            //inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , newSocket , inet_ntoa(serverAddr.sin_addr) , ntohs(serverAddr.sin_port));
            
            //send new connection greeting message
            // if( send(newSocket, "hi", strlen("hi"), 0) != strlen("hi") ) 
            // {
            //     perror("send");
            // }

              
            puts("Welcome message sent successfully");
              
            //add new socket to array of sockets
            for (i = 0; i < num_clients; i++) 
            {
                //if position is empty
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = newSocket;
                    printf("Adding to list of sockets as %d\n" , i);
                     
                    break;
                }
            }
        }

         //else its some IO operation on some other socket :)
        for (i = 0; i < num_clients; i++) 
        {
            sd = client_socket[i];
              
            if (FD_ISSET( sd , &readfds)) 
            {
                int client_number = i;

                //Check if it was for closing , and also read the incoming message
                valread = read( sd , buffer, 1024);
                if(valread == 0)
                {
                    printf("%s\n", buffer);
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&serverAddr , (socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(serverAddr.sin_addr) , ntohs(serverAddr.sin_port));
                      
                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    client_socket[i] = 0;
                }
                buffer[valread] = '\0';
                if (strstr(buffer, "#password:") != NULL && strstr(buffer, "#name:")) {
                    buffer[valread] = '\0';
                    send(sd , buffer , strlen(buffer) , 0 );
                    bzero(buffer, sizeof(buffer));
                }
                else{
                    buffer[valread] = '\0';

                    char notify[1024];
                    snprintf(notify, sizeof notify, "\nMESSAGE RECIEVED FROM CLIENT # %d:", client_number);
                    notify[sizeof(notify) - 1] = '\0';

                    // THIS IS THE SEND
                    for(k=0;k<num_clients;k++){
                        sd = client_socket[k];
                        //send message

                        //uncomment this for testing
                        //printf("Sending \"%s\" to client_socket[%d]\n", buffer, k);

                        //print which client sent the message
                        send(sd, notify, strlen(notify), 0);
                        //print the message itself
                        send(sd , buffer , strlen(buffer) , 0 );
                    }

                    bzero(buffer, sizeof(buffer));

                }
            
                //set the string terminating NULL byte on the end of the data read
                
                
                // printf("%i\n", valread);
                // printf("%s\n", buffer);
                // send(sd , buffer , strlen(buffer) , 0 );
                // bzero(buffer, sizeof(buffer));
            }
        }
   


    }


    return 0;
}

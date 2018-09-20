#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char* deblank(char* input)                                         
{
    int i,j;
    char *output=input;
    for (i = 0, j = 0; i<strlen(input); i++,j++)          
    {
        if (input[i]!=' ' && input[i]!='\n')                           
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

	int sockfd, ret, newSocket;
	int port = atoi(argv[1]);   //Check for Error
	int num_clients = atoi(argv[2]);   // Check for Errors
	struct sockaddr_in serverAddr;
	struct sockaddr_in newAddr;
	socklen_t addr_size;
	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("Error in connection.\n");
		exit(1);
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

	if(listen(sockfd, num_clients) == 0){
		printf("Listening...\n");
	}else{
		printf("Error in binding.\n");
	}



	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);

			while(1){
				recv(newSocket, buffer, 1024, 0);
				
				if(strcmp(deblank(buffer), "#EXIT") == 0){
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}else{
					// printf("Client: %s\n", buffer);
					send(newSocket, deblank(buffer), strlen(buffer), 0);
					bzero(buffer, sizeof(buffer));
					printf("%s", deblank(buffer));
				}
			}
		}

	}

	close(newSocket);


	return 0;
}



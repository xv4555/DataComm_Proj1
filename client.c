#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int read_server_response(){
    int fd = 0;
    char buff[1024];
    int in;

    /* Now read server response */
    bzero(buff,256);
    in = recv(fd,buff,255,0);
    if (in < 0) 
    {
        perror("\nClient Error: Reading from Server");
        return 0;
    }
    printf("\nReceived FROM SERVER: %s ",buff);
    return 1;
}

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

// function for prepending strings
void prepend(char* s, const char* t)
{
    size_t len = strlen(t);
    size_t i;

    memmove(s + len, s, strlen(s) + 1);

    for (i = 0; i < len; ++i)
    {
        s[i] = t[i];
    }
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("Usage: ./351ChatClient <ip> <port> <name> <password>\n");
        printf("None of the 4 required arguments should contain spaces.\n");
    }

    // prepend #name: to name
    char* name = argv[3];
    prepend(name, "#name:");

    // prepend #password: to password
    char* password = argv[4];
    prepend(password, "#password:");

    //concatenate name and password
    strncat(name, password, (sizeof(name) - strlen(password)) );

    int fd = 0;
    char buff[1024];

    //Setup Buffer Array
    memset(buff, '0',sizeof(buff)); 

    //Create Socket
    fd = socket(AF_INET, SOCK_STREAM, 0);
        if(fd<0)
    {
        perror("Client Error: Socket not created succesfully");
        return 0;
    }
    
    //Structure to store details
    struct sockaddr_in server; 
    memset(&server, '0', sizeof(server)); 

    //Initialize    
    server.sin_family = AF_INET;
    server.sin_port =  htons(atoi(argv[2]));

    int in = inet_pton(AF_INET, argv[1], &server.sin_addr);
    if(in<0)
    {
        perror("Client Error: IP not initialized succesfully");
        return 0;
    }

    //Connect to given server
    in = connect(fd, (struct sockaddr *)&server, sizeof(server));
    if(in<0)
    {
        perror("Client Error: Connection Failed.");
        return 0;
    }       

    // send name and password
    printf("Sending name and password...\n");

    // copy the name (which is the concatenated name & password)
    // char* into the char[] for sending
    strncpy(buff, name, sizeof(buff - 1));
    buff[sizeof(buff)-1] = '\0';

    // send the char[] with the name
    in = send(fd,buff,strlen(buff),0);
    if (in < 0) 
    {
     perror("\nClient Error: Writing to Server");
        return 0;
    }

//TODO
///////
    /* Now read server response */
    //TRY TO REPLACE THIS WITH SERVER LISTENING FUNCTION AT THE BOTTOM
    bzero(buff,256);
    in = recv(fd,buff,255,0);
    if (in < 0) 
    {
        perror("\nClient Error: Reading from Server");
        return 0;
    }
    printf("\nReceived FROM SERVER: %s ",buff);

//*****
// dont need the below commented out code because we only need to
// send the name which contains the name & password
// (since we concatenated them into name)

    // TODO WHY DOESNT THIS WORK HERE?
    //if (read_server_response() == 0){return 0;}
///////

//     // copy the password char* into the char[]
//     strncpy(buff, password, sizeof(buff - 1));
//     buff[sizeof(buff)-1] = '\0';

//     // send the char[] with the password
//     in = send(fd,buff,strlen(buff),0);
//     if (in < 0) 
//     {
//      perror("\nClient Error: Writing to Server");
//         return 0;
//     }

// //TODO
// ///////
//     /* Now read server response */
//     //TRY TO REPLACE THIS WITH SERVER LISTENING FUNCTION AT THE BOTTOM
//     bzero(buff,256);
//     in = recv(fd,buff,255,0);
//     if (in < 0) 
//     {
//         perror("\nClient Error: Reading from Server");
//         return 0;
//     }
//     printf("\nReceived FROM SERVER: %s ",buff);

//     // TODO WHY DOESNT THIS WORK HERE?
//     //if (read_server_response() == 0){return 0;}
// ///////

    // infinite loop to send and read in messages
    while(1)
    {
        printf("\n>");
        bzero(buff,256);
        fgets(buff,255,stdin);


        /* Send message to the server */
        in = send(fd,deblank(buff),strlen(buff),0);
        if (in < 0) 
        {
         perror("\nClient Error: Writing to Server");
            return 0;
        }
        
        // check if user put "#EXIT" and exit.
        if(buff[0] == '#' &&
           buff[1] == 'E' &&
           buff[2] == 'X' &&
           buff[3] == 'I' &&
           buff[4] == 'T')
        {
            printf("%s\n", "Bye! Thanks for chatting!");
            return 1;
        }

        /* Now read server response */
        bzero(buff,256);
        in = recv(fd,buff,255,0);
        if (in < 0) 
        {
            perror("\nClient Error: Reading from Server");
            return 0;
        }
        printf("%s",buff);
    }
    close(fd);
    return 0;
}


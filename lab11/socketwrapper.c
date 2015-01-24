#include<stdlib.h>
#include<sys/types.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

#include "socketwrapper.h"

int start_server(int port)
{
    int serverfd;
    struct sockaddr_in servaddr;
    
    serverfd = socket(PF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(serverfd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr_in))<0)
    {
        printf("Bind ERROR\n");
        exit(1);
    }
    return serverfd;	
}


int start_client(int port)
{
    int clientfd;
    struct sockaddr_in servaddr;
    int len = sizeof(servaddr);

    clientfd = socket(PF_INET, SOCK_STREAM, 0);
    
   
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(port);

    if(connect(clientfd, (struct sockaddr *)&servaddr, (socklen_t)len) < 0)
    {
        printf("Connect ERROR!\n");
    }
    
    return clientfd;
}

void closefd(int fd)
{
    close(fd);
}

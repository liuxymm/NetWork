#include<pthread.h>
#include<netinet/in.h> 
#include<sys/types.h>  
#include<sys/socket.h>  
#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>

#include"file_trans.h"

#define DEFAULT_PORT 8899

void file_send(int clientfd,int serverfd,char *filename)
{
    struct trans_packet tpacket;
    struct sockaddr_in clientaddr;
    int thread_num;
    int filesize;
    int thread_size;

    filesize = getfilesize(filename);

    if(filesize <= THREAD_LOAD)
    {
        thread_num = 1;      
    }    
    else
    {
        thread_num = filesize / THREAD_LOAD + 1;

        if(thread_num > 8) 
        {
            thread_num = 8;
        }
    }

    if(filesize <= 1024*1024*1024)
    {
        thread_size = THREAD_LOAD;
    }
    else
    {
        thread_size = (filesize+thread_num)/thread_num; 
    }


    int n;
    n = send(clientfd,(char*)&thread_num,4,0);

    pthread_t *ptd;
    ptd = (pthread_t *)malloc(thread_num*sizeof(pthread_t));

    int i,size = sizeof(clientaddr); 
   
    listen(serverfd,100);

    for(i = 0 ; i < thread_num ; i ++)
    {
        
        struct thread_info *thread_args;
   
        thread_args = (struct thread_info*)malloc(sizeof(struct thread_info));

        int newclientfd = accept(serverfd, (struct sockaddr*)&clientaddr,&size);
           
        strcpy(thread_args->filename,filename); 
        thread_args->seekpos = i*thread_size; 
        thread_args->socketfd = newclientfd;

        if((i+1)*thread_size > filesize)
        {
            thread_args->size = filesize - i * MAXSIZE;

            if(thread_args->size == 0)
            {
                break;
            } 
        } 
        else
        {
            thread_args->size = thread_size; 
        }

        if(pthread_create((ptd+i),NULL,send_thread_serve,(void*)thread_args) != 0)
        {
             printf("create thread %d ERROR\n",i);
        }
    }

    for(i = 0; i < thread_num; i++)
    {
        pthread_join(ptd[i], NULL);
    } 
     
}

int main(int argc, char **argv) 
{ 
    char filename[FILE_NAME_MAX_SIZE];
    int serverfd,clientfd;
    int port = DEFAULT_PORT;

    if(argc > 1)  
    {
        port = atoi(argv[1]);
    }
    
    serverfd = start_server(port); 
  
    if(serverfd < 0) 
    { 
        perror("Create Socket ERROR"); 
        exit(1); 
    } 
    
   

    while(1)
    {
        clientfd = filerequestrecv(serverfd,filename);

        printf("fileanme %s\n",filename); 

        file_send(clientfd,serverfd,filename);
    }

    close(serverfd); 
    return 0; 
} 

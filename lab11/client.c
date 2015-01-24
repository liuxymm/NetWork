#include<pthread.h>
#include<netinet/in.h> 
#include<sys/types.h>  
#include<sys/socket.h>  
#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include<time.h>

#include"file_trans.h"

#define DEFAULT_PORT 8899


void file_recv(int clientfd,char *filename,char*savefilename,int port)
{
    FILE *fp;
    fp = fopen(savefilename, "w+");
    close(fp);
    
    struct trans_packet tpacket;
    struct sockaddr_in clientaddr;
    int thread_num,n;
    time_t t_start,t_end; 
    
    t_start=time(NULL);

    n = recv(clientfd,(char*)&thread_num,4,0);

    pthread_t *ptd;
    ptd = (pthread_t *)malloc(thread_num*sizeof(pthread_t));
    
    int i;
    for(i = 0 ; i < thread_num ; i ++)
    {
       
        struct thread_info *thread_args;
   
        thread_args = (struct thread_info*)malloc(sizeof(struct thread_info));

        int newclientfd = start_client(port);

        strcpy(thread_args->filename,savefilename); 
        thread_args->seekpos = -1;
        thread_args->socketfd = newclientfd;
        thread_args->size = -1;
      
        if(pthread_create((ptd+i),NULL,recv_thread_serve,(void*)thread_args) != 0)
        {
             printf("create thread %d ERROR\n",i);
        }
    }
    
    for(i = 0; i < thread_num; i++)
    {
        pthread_join(ptd[i], NULL);
    } 

    t_end=time(NULL);

    printf("time : %.0fs\n",difftime(t_end,t_start));

}

int main(int argc, char **argv) 
{ 
    char filename[FILE_NAME_MAX_SIZE],savefilename[FILE_NAME_MAX_SIZE];
    int serverfd,clientfd;
    int port = DEFAULT_PORT;
 

    if(argc > 1)  
    {
        port = atoi(argv[1]);
    }
    
    printf("Get file name:  ");
    scanf("%s",filename);

    printf("savefilename:  ");
    scanf("%s",savefilename);

    
    
    clientfd = start_client(port); 
   
    filerequestsend(clientfd,filename);

    file_recv(clientfd,filename,savefilename,port);
    
    close(clientfd); 
  
    return 0; 
} 

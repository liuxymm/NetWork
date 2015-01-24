#include<stdlib.h>
#include<sys/types.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

#include"file_trans.h"

/* get file size*/
int getfilesize(char *filename)
{
    FILE *fp ;   
    int size = -1;    

    if((fp = fopen(filename,"r"))== NULL)
    {
        printf("File: %s Open Error\n",filename);
    }  

    fseek(fp,0L,SEEK_END);
    size = ftell(fp);
    fclose(fp);

    return size;
}

/* send file request*/
void filerequestsend(int fd,char *filename)
{
    send(fd,filename,strlen(filename),0);
}

/* receive file request*/
int filerequestrecv(int fd,char *filename)
{
    int clientfd;
    struct sockaddr_in clientaddr;
    int len = sizeof(clientaddr);
    int n;

    listen(fd,10);

    clientfd = accept(fd, (struct sockaddr*)&clientaddr, &len);
   
    if((n = recv(clientfd,filename,FILE_NAME_MAX_SIZE,0)) < 0)
    {
        printf("recv filename error\n");
        return -1;
    }
    filename[n] = '\0';

    return clientfd;
}




void *send_thread_serve(void *thread_args)
{
    struct thread_info *thread_info;
    thread_info = (struct thread_info *)thread_args;

    int clientfd = thread_info->socketfd;
    int seekpos = thread_info->seekpos;
    char *filename = thread_info->filename;
    int size = thread_info->size;
    char buffer[TRANS_SIZE];
    
    memset(buffer,'\0',sizeof(buffer)); 
    //printf("ss %d  %s\n",size,filename);
    
    FILE *fp;

    if((fp = fopen(filename,"r")) == NULL)
    {
        printf("file is not exist!\n");
        return;
    } 

    int i;
    int n;
    int transferedsize = 0;
    for(i = 0 ; transferedsize < size ; i++) 
    { 
        fseek(fp,seekpos,SEEK_SET);
    
        struct trans_packet *tpack;
        tpack = (struct trans_packet*)malloc(sizeof(struct trans_packet));
      
        if(size - transferedsize > MAXSIZE)
        { 
            tpack->size = MAXSIZE;
        }
        else
        {
            tpack->size = size - transferedsize;
        }
       
        if(!tpack->size)
        {
            break; 
            
        } 

        if(fread(tpack->buffer,sizeof(char),tpack->size,fp) > 0)
        {        
            tpack->seekpos = seekpos;
            memcpy(buffer,tpack,sizeof(*tpack));
            n = send(clientfd,buffer,sizeof(buffer),0);
        }
        else
        {
 
            break;
        }

        seekpos += tpack->size;
        transferedsize += tpack->size;   
        free(tpack);
    }  


    
    char resp[TRANS_SIZE];

    struct trans_packet *overpack;
    overpack = (struct trans_packet*)malloc(sizeof(struct trans_packet));
    strcpy(overpack->buffer,"over");  
    memcpy(resp,overpack,sizeof(*overpack));
    send(clientfd,resp,sizeof(resp),0);


    closefd(clientfd);
    fclose(fp);
}


void *recv_thread_serve(void *thread_args)
{
    struct thread_info *thread_info;
    thread_info = (struct thread_info *)thread_args;

    int clientfd = thread_info->socketfd;
    char *filename = thread_info->filename;
    char buffer[TRANS_SIZE];
    
    memset(buffer,'\0',sizeof(buffer)); 
    

    FILE *fp;

    if((fp = fopen(filename,"r+")) == NULL)
    {
        printf("file is not exist!\n");
        return;
    } 
    
    int i = 0;
    while(1)
    {
        struct trans_packet *tpack;
        tpack = (struct trans_packet*)malloc(sizeof(struct trans_packet));
    
        int n; 
        n = recv(clientfd,buffer,sizeof(buffer),0);
        if(n <= 0)
        {
            break;
        }
	int last = MAXSIZE + 2*sizeof(int) - n;
	while(last > 0 )
	{
		n = recv(clientfd,buffer[last],last,0);
		last = last - n
	}


        memcpy(tpack,buffer,sizeof(*tpack));

        if(!strncmp(tpack->buffer,"over",strlen("over")))
        {
            break;
        }

        fseek(fp,tpack->seekpos,SEEK_SET);
        fwrite(tpack->buffer, sizeof(char), tpack->size, fp);
        free(tpack);
    }

 

    closefd(clientfd);
    fclose(fp);
    return;
}


#ifndef  _FILE_TRANS_H_
#define  _FILE_TRANS_H_

#define MAXSIZE (1024*8)
#define FILE_NAME_MAX_SIZE 512 
#define THREAD_LOAD  (1024*1024*128*4)
#define TRANS_SIZE (sizeof(struct trans_packet)+4)

typedef struct trans_packet
{
    int seekpos;
    int size;
    char buffer[MAXSIZE];
    
}*transinfo; 

typedef struct thread_info
{
     char filename[FILE_NAME_MAX_SIZE];
     int seekpos;
     int socketfd;
     int size;
}*threadinfo;

/* get file size*/
int getfilesize(char *filename);

/* send file request*/
void filerequestsend(int fd,char *filename);

/* receive file request*/
int filerequestrecv(int fd,char *filename);

void *send_thread_serve(void *thread_args);

void *recv_thread_serve(void *thread_args);

#endif

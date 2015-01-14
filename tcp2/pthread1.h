#include<pthread.h>
#include<sys/socket.h>
#include<unistd.h>
#define NTHREAD 10
typedef struct 
{
      pthread_t tid;
      long    thread_count;
}Thread;


Thread *tptr;

struct Buffer
{
	char buffer[1024];
	int  offset;
}buffer;



int listenfd;
socklen_t addrlen;
pthread_mutex_t mutex;

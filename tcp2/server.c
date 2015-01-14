/*************************************************************************
	> File Name: server.c
	> Author: Liuxy
	> Mail: 1518487798@qq.com 
	> Created Time: 2015年01月10日 星期六 19时48分28秒
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<sys/socket.h>
#include<sys/types.h>
//#include<epoll.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>

#include"pthread1.h"

#define PORT 8000
#define LISTENQ 20
#define ADDRLEN 30
#define NTHREAD 10
/*client不用多线程产生多个请求啊，服务器直接用多线程传递指定位置文件就可以了*/
//设置文件目录
//char sFilePath[40]="";
//将buffer改为结构体，里面包含里定位文件的位置
//char buffer[1024];


// 线程任务函数

void *thread_make(int i);
void *thread_m(void *i);
void *request_handle(int fd, char * client_info,int port,int i);


/*struct client_thread_info
{
	char addr[ADDRLEN];
	int port;
	int fd;
};
*/


void *thread_make(int i)//i 号线程
{
	pthread_create(&tptr[i].tid,NULL,(void *)&thread_m,(void *)i);//tid[] shengming
	printf("%d thread tid[%x]\n",i,tptr[i].tid);
	return;
}

void *thread_m(void *arg)//arg为线程号ｉ，不是tid
{
	int connfd;
	socklen_t clilen=sizeof(addrlen);
	struct sockaddr_in cliaddr;
	memset(&cliaddr,0,sizeof(cliaddr));
	
	printf("thread [%d] is running \n",(int )arg);
	while(1)
	{
		pthread_mutex_lock(&mutex);
		connfd=accept(listenfd,(struct sockaddr *)&cliaddr,&clilen);
		pthread_mutex_unlock(&mutex);
		tptr[(int)arg].thread_count++;

		//提取clientaddr的信息，调用handler
		int clientfd=connfd;
		int port=ntohs(cliaddr.sin_port);
		char *client_info;
	    strcpy(client_info,inet_ntoa(cliaddr.sin_addr));
		//与主线程分离
		pthread_detach(pthread_self());

		// handle
		request_handle(clientfd,client_info,port,arg);/*tptr[(int)arg].tid*/

		close(connfd);

		
	}

}
/*第ｉ号线程读取起始地址为100M*i处内容*/
void *request_handle(int fd,char *client_info,int port,int i)//
{
	FILE *stream;
	int len=1024*1024*1024/10,n;
	if(stream=fopen("data1","r")==NULL)
	{
		printf("open failed!\n");
		exit(1);
	}
	int start=i*len;
	if(fseek(stream,start,0)==-1)
	{
		printf("fseek failed\n");
		exit(1);
	}
       
	while(len)
	{
		memset(&buffer,0,sizeof(buffer));//////
		//read
		if(n=fread(buffer.buffer,1,1024,stream)<0)
			printf("read failed!\n");
		else
 		{
			printf("transfering size=%d\n",n);
         
		}
		 buffer.offset=start;	
		//send
		if(send(fd,&buffer,n,0)<0)
			printf("sending failed!\n");
		else 
			printf("sending!\n");
		len-=n;
	}
}



int main()
{
	int listenfd,sockfd,i;
	//socket
	struct sockaddr_in clientaddr;
	struct sockaddr_in serveraddr;
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	

	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(PORT);
        serveraddr.sin_addr.s_addr= htonl(INADDR_ANY);//自动获取服务器主机的ＩＰ地址
	

	if((bind(listenfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr)))==-1)
	{
		perror("binding to socket\n");
		exit(0);
	}

	if(listen(listenfd,LISTENQ)==-1)
	{
		perror("listening to socket\n");
		exit(0);
	}

	printf("ready for req\n");
    tptr=(Thread *)calloc(10,sizeof(Thread));//calloc  可以初始化为０
      //  while(1)
	//	{
		//	iptr=(int *)malloc(sizeof(int));

		/*	connfd=accept(listenfd,(struct sockaddr *)clientaddr,&size);

			printf("accepting a client from %s port %d\n",(char *)inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
			struct client_thread_info *cli;

			strcpy(cli->addr,(char *)inet_ntoa(clientaddr.sin_addr));
			cli->port=ntohs(clientddr.sin_port);
			cli->fd=connfd;
		*/
			//
			//预先创建一个线程池
			for(i=0;i<10;i++)
			{
				thread_make(i);
			}
		/*	pthread_t tid;
			if(pthread_create(&tid,NULL,svm,(void *)cli)!=0)
			{
				printf("pthread create error\n");
			}*/
	//	}

        

	
}


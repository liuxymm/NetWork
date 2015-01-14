/*************************************************************************
	> File Name: client.c
	> Author: Liuxy
	> Mail: 1518487798@qq.com 
	> Created Time: 2015年01月13日 星期二 09时43分10秒
 ************************************************************************/


#include <netinet/in.h>    // for sockaddr_in
#include <sys/types.h>    // for socket
#include <sys/socket.h>    // for socket
#include <stdio.h>        // for printf
#include <stdlib.h>        // for exit
#include <string.h>        // for bzero
#include <arpa/inet.h>
#include <pthread.h>
#include "pthreadc.h"
 
#define PORT    7754
#define BUFFER_SIZE 1028
int port;///sha ????
void *make_thread(int i);
void *m_thread(void *args);
void *handle(int clientfd);


void *make_thread(int i)
{
	//void *thread_main(void *);
	
	
	pthread_create(&threadc[i].tid,NULL,(void *)&m_thread,(void *)i);
	printf("%d thread tid[%x]\n",i,threadc[i].tid);
	return;
}


void *m_thread(void *args)
{
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	if(inet_aton("127.0.0.1",&server_addr.sin_addr) == 0) //服务器的IP地址来自程序的参数
	{
		printf("Server IP Address Error!\n");
		exit(1);
	}
	server_addr.sin_port = htons(port);
	socklen_t server_addr_length = sizeof(server_addr);
        
	//printf("thread %u is running!\n",threadc[(int)args.i].tid);
	
	
	//客户端多个线程与服务器建立链接用不用设临界去呀？我觉得应该不用吧
	//int fd;
	//fd=(args)->fd;
	if(connect(client_fd,(struct sockaddr *)&server_addr,server_addr_length)<0)
	{
		printf("connect failed!\n");
		exit(1);
	}
	pthread_detach(pthread_self());
		//向服务器发送数据，准备接受数据
		//不发送数据可否？
	handle(client_fd);
	
	close(client_fd);

	

}
void *handle(int clientfd)
{
	FILE *stream;
	int len;
	struct Buf buf;
	if(stream=fopen("data","w+t")==NULL)
	{
		printf("open 'data' failed!\n");
	}
	//while(len=recv(clientfd,Buf,BUFFER_SIZE,0))
	//{
	len=recv(clientfd,&buf,sizeof(struct Buf),0);

	if(len<0)
	  { printf("recv failed!\n");
        }
	if(-1==fseek(stream,buf.offset,0))
	{
		printf("fseek failed!\n");
		exit(0);
	}
	if(fwrite(buf.buf,sizeof(char),BUFFER_SIZE,stream)<len)
	{
		printf("fwrite failed!\n");
		exit(0);
	}
	
	//}
	fclose(stream);
}



int main(int argc, char **argv)
{

/*	if (argc != 2)
	{
		printf("Usage: ./%s ServerIPAddress\n",argv[0]);
		exit(1);
	}
*/
        int port = PORT;
	if (argc > 1)
	{
		port = atoi(argv[1]);
	}
 

	
        int i=0;
	
	//设置一个socket地址结构client_addr,代表客户机internet地址, 端口
	struct sockaddr_in client_addr;
	bzero(&client_addr,sizeof(client_addr)); 
	client_addr.sin_family = AF_INET;   
	client_addr.sin_addr.s_addr = htons(INADDR_ANY);
	client_addr.sin_port = htons(PORT);    

	//创建用于internet的流协议(TCP)socket,用client_fd代表客户机socket
	int client_fd = socket(AF_INET,SOCK_STREAM,0);
	if( client_fd < 0)
	{
		printf("Create Socket Failed!\n");
		exit(1);
	}
	//把客户机的socket和客户机的socket地址结构联系起来
	if( bind(client_fd,(struct sockaddr*)&client_addr,sizeof(client_addr)))
	{
		printf("Client Bind Port Failed!\n");
		exit(1);
	}
 
	//设置一个socket地址结构server_addr,代表服务器的internet地址, 端口
	

	/*这里应该是多个线程与ｓｅｒｖｅｒ发起链接，可以有多个ｓｅｒｖｅｒ线程与ｃｌｉｅｎｔ建立链接
　　　　　但是对于accept返回已连接套接字只能有一个线程进入临界区
         向服务器发起连接,连接成功后client_fd代表了客户机和服务器的一个socket连接
　　　　　　　　*/
         //建立线程池
	
	
        for(i=0;i<10;i++)
	{
		//cli.i=i;
		make_thread(i);
	}
	
 
	return 0;
}

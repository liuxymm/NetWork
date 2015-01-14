/*************************************************************************
	> File Name: pthreadc.h
	> Author: Liuxy
	> Mail: 1518487798@qq.com 
	> Created Time: 2015年01月13日 星期二 10时20分48秒
 ************************************************************************/
#include<stdio.h>
//#include<iostream>
#include<pthread.h>
//using namespace std;

struct Threadc
{
	pthread_t tid;
	int thread_count;
}*threadc;

struct Buf
{
	char buf[1024];
	int offset;
};

struct cli_info
{
	int fd;
	//pthread_t tid;
	int i;
};
int client_fd;

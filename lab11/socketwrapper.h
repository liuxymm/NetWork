#ifndef _SOCKET_WRAPER_H_
#define _SOCKET_WRAPER_H_

/* start server*/
int start_server(int port);

/* start client*/
int start_client(int port);

/* close connect handler*/
void closefd(int fd);

#endif

#ifndef _SOCKET_H
#define _SOCKET_H

int tcp_connect(char* host, int port);
int tcp_recv(int sd, unsigned char* buf, int length);

#endif

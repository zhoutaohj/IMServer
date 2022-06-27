#include <iostream>
#include <sys/select.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <unistd.h>
#include <stdio.h>

#define SERVERPORT 10000
ssize_t readn(int fd,void * buf,size_t nbytes);
ssize_t writen(int fd,const void * buf,size_t nbytes);

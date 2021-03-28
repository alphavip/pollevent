#ifndef _UTILS_H_
#define _UTILS_H_



#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include<unistd.h>

#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)
#endif

    typedef int socket_t;


#endif // _UTILS_H_


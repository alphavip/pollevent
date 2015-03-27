#ifndef _UTILS_H_
#define _UTILS_H_

#ifdef _WIN32
#include <windows.h>
#include <winsock2.h>
#ifndef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK
#define EINPROGRESS WSAEINPROGRESS
#define ECONNRESET WSAECONNRESET
#define ENETRESET WSAENETRESET
#endif
#ifndef ENOMEM
#define EINTR WSAEINTR
#define EFAULT WSAEFAULT
#define ENOMEM WSA_NOT_ENOUGH_MEMORY
#define EAGAIN WSATRY_AGAIN
#endif
#ifndef EADDRINUSE
#define EADDRINUSE WSAEADDRINUSE
#endif

#ifndef HAVE_SOCKLEN_T
#define HAVE_SOCKLEN_T
typedef int socklen_t;
#endif

typedef intptr_t socket_t;

#else

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>

#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)
#endif

typedef int socket_t;

#endif


#endif // _UTILS_H_


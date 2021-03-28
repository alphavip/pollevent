#include "Config.h"
#include "TcpSocket.h"
#include "Utils.h"

#include <thread>

#ifndef _WIN32
#include <errno.h>
#endif


TcpSocket::TcpSocket(): Socket(AF_INET, SOCK_STREAM, 0), _pendClose(false)
{
}

TcpSocket::TcpSocket( socket_t fd ): Socket(fd), _pendClose(false)
{
}

bool TcpSocket::connect( const char * addr, uint16_t port )
{
    uint32_t ipaddr = resolveAddress(addr);
    if(ipaddr == 0xFFFFFFFF)
        return false;
    return connect(ipaddr, port);
}

bool TcpSocket::connect( uint32_t ipaddr, uint16_t port )
{
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(ipaddr);
    saddr.sin_port = htons(port);
    if(::connect(_fd, (const struct sockaddr *)&saddr, sizeof(saddr)) == SOCKET_ERROR && (lastErr() != EINPROGRESS && lastErr() != EWOULDBLOCK))
        return false;
    return true;
}

int TcpSocket::read( void * buf, int len )
{
#ifdef _WIN32
    return ::recv(_fd, (char *)buf, len, 0);
#else
    return ::read(_fd, buf, len);
#endif
}

int TcpSocket::write( const void * buf, int len )
{
#ifdef _WIN32
    return ::send(_fd, (const char *)buf, len, 0);
#else
    return ::write(_fd, buf, len);
#endif
}




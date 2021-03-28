#include "Config.h"

#include "Socket.h"

#include "Utils.h"



Socket::Socket(): _fd(-1)
{
}

Socket::Socket( socket_t fd ): _fd(fd)
{
}

Socket::Socket( int af, int type, int protocol ): _fd(-1)
{
    init(af, type, protocol);
}

Socket::~Socket()
{
    close();
}

bool Socket::init( int af, int type, int protocol )
{
    _fd = ::socket(af, type, protocol);
    return _fd >= 0;
}

bool Socket::bind( const char * addr, uint16_t port )
{
    uint32_t ipaddr = resolveAddress(addr);
    if(ipaddr == 0xFFFFFFFF)
        return false;
    return bind(ipaddr, port);
}

bool Socket::bind( uint32_t ipaddr, uint16_t port )
{
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(ipaddr);
    saddr.sin_port = htons(port);
    if(::bind(_fd, (const struct sockaddr *)&saddr, sizeof(saddr)) == SOCKET_ERROR)
        return false;
    return true;
}

bool Socket::bind( uint16_t port )
{
    return bind(static_cast<uint32_t>(INADDR_ANY), port);
}

bool Socket::listen( int backlog )
{
    return ::listen(_fd, backlog) >= 0;
}

void Socket::shutdown( int sf )
{
    if(_fd < 0)
        return;
    ::shutdown(_fd, sf);
}

void Socket::close()
{
    if(_fd < 0)
        return;

    ::close(_fd);
}

bool Socket::makeKeepAlive( int idle, int intv, int cnt )
{
    if(idle < 0)
    {
        int keepAlive = 0;
        if(setsockopt(_fd, SOL_SOCKET, SO_KEEPALIVE, (const char *)&keepAlive, sizeof(keepAlive)) == SOCKET_ERROR)
            return false;
    }
    else
    {
        int keepAlive = 1;
        if(setsockopt(_fd, SOL_SOCKET, SO_KEEPALIVE, (const char *)&keepAlive, sizeof(keepAlive)) == SOCKET_ERROR)
            return false;
#ifndef _WIN32
#ifdef SYSTEM_MACOS
        if(setsockopt(_fd, IPPROTO_TCP, TCP_KEEPALIVE, (const char *)&idle, sizeof(idle)) == SOCKET_ERROR)
            return false;
#else
        if(setsockopt(_fd, IPPROTO_TCP, TCP_KEEPIDLE, (const char *)&idle, sizeof(idle)) == SOCKET_ERROR)
            return false;
        if(setsockopt(_fd, IPPROTO_TCP, TCP_KEEPINTVL, (const char *)&intv, sizeof(intv)) == SOCKET_ERROR)
            return false;
        if(setsockopt(_fd, IPPROTO_TCP, TCP_KEEPCNT, (const char *)&cnt, sizeof(cnt)) == SOCKET_ERROR)
            return false;
#endif
#endif
    }
    return true;
}

bool Socket::makeLinger( bool enabled )
{
    struct linger so_linger;
    so_linger.l_onoff = enabled ? 1 : 0;
    so_linger.l_linger = 0;
    if(setsockopt(_fd, SOL_SOCKET, SO_LINGER, (const char *)&so_linger, sizeof(so_linger)) == SOCKET_ERROR)
        return false;
    return true;
}

bool Socket::makeBlock( bool enabled )
{
#ifdef _WIN32
    u_long nonblocking = enabled ? 0 : 1;
    if (::ioctlsocket(_fd, FIONBIO, &nonblocking) == SOCKET_ERROR) {
        return false;
    }
#else
    int flags;
    if ((flags = ::fcntl(_fd, F_GETFL, NULL)) < 0) {
        return false;
    }
    if(enabled)
    {
        if (::fcntl(_fd, F_SETFL, flags & ~O_NONBLOCK) == -1) {
            return false;
        }
    }
    else
    {
        if (::fcntl(_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
            return false;
        }
    }
#endif
    return true;
}

bool Socket::makeNoDelay( bool enabled )
{
    int yes = enabled ? 1 : 0;
    return setsockopt(_fd, IPPROTO_TCP, TCP_NODELAY, (const char *)&yes, sizeof(yes)) == -1;
}

int Socket::lastErr()
{
#ifdef _WIN32
    return WSAGetLastError();
#else
    return errno;
#endif
}

uint32_t Socket::resolveAddress( const char * addr )
{
    struct in_addr ina;
#ifdef _WIN32
    ina.s_addr = inet_addr(addr);
    if (ina.s_addr == INADDR_NONE) {
#else
    if (inet_aton(addr, &ina) == 0) {
#endif
        struct hostent *he;

        he = gethostbyname(addr);
        if (he == NULL) {
            return 0xFFFFFFFF;
        }
        return ntohl(*(uint32_t *)he->h_addr);
    }
    return ntohl(ina.s_addr);
}

uint32_t Socket::getAddr()
{
    struct sockaddr_in addr = {0};
    socklen_t len = sizeof(struct sockaddr_in);
    if(getsockname(_fd, (struct sockaddr *)&addr, &len) < 0)
        return 0xFFFFFFFF;
    return ntohl(addr.sin_addr.s_addr);
}

uint32_t Socket::getRemoteAddr()
{
    struct sockaddr_in addr = {0};
    socklen_t len = sizeof(struct sockaddr_in);
    if(getpeername(_fd, (struct sockaddr *)&addr, &len) < 0)
        return 0xFFFFFFFF;
    return ntohl(addr.sin_addr.s_addr);
}




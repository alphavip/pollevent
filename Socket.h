#ifndef _SOCKET_H_
#define _SOCKET_H_

#include "Utils.h"

namespace sys
{

/* Basic socket class to implement platform independent interfaces */
class Socket
{
public:
    /* Constructors & desctructors */
    Socket();
    Socket(socket_t fd);
    Socket(int af, int type, int protocol);
    virtual ~Socket();
    bool init(int af, int type, int protocol);
    /* Bind to host:port */
    bool bind(const char * host, uint16_t port);
    /* Bind to addr:port */
    bool bind(uint32_t addr, uint16_t port);
    /* Bind to 0.0.0.0:port */
    bool bind(uint16_t port);
    /* Start listening */
    bool listen(int backlog);
    /* Shutdown (aka graceful close) */
    void shutdown(int sflag);
    /* Close (aka hard close, reset connection) */
    void close();

    /* set keepalive of socket */
    bool makeKeepAlive(int, int, int);
    /* set linger of socket */
    bool makeLinger(bool);
    /* set block of socket */
    bool makeBlock(bool);
    /* set nodelay */
    bool makeNoDelay(bool);

    /* get socket address */
    uint32_t getAddr();
    /* get socket remote address */
    uint32_t getRemoteAddr();


public:
    /* get last error */
    static int lastErr();
    /* resolve ip address from string */
    static uint32_t resolveAddress(const char *);

public:
    /* cast to fd */
    inline operator socket_t() { return _fd; }
    /* Check if fd is initilized */
    inline bool initialized() { return _fd >= 0; }

protected:
    socket_t _fd;
};

}

#endif // _SOCKET_H_


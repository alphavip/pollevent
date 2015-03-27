#ifndef _TCPSOCKET_H_
#define _TCPSOCKET_H_

#include "Socket.h"

namespace sys
{

/* TCP-Socket class to do basic actions */
class TcpSocket: public Socket
{
public:
    /* Create a new socket */
    TcpSocket();
    /* Assign to an available socket/fd */
    TcpSocket(socket_t fd);
    /* Connect to host:port */
    bool connect(const char * host, uint16_t port);
    /* Connect to addr:port */
    bool connect(uint32_t addr, uint16_t port);
    /* read bytes from socket */
    int read(void *, int);
    /* write bytes into socket */
    int write(const void *, int);
    /* Add a pend close flag */
    void pendClose() { _pendClose = true; }
    /* Check if pend close is set */
    inline bool isPendClose() { return _pendClose; }

private:
    bool _pendClose;
};

/* TCP-Listener class */
class TcpListener: public TcpSocket
{
public:
    /* Listen at host:port */
    TcpListener(const char * host, uint16_t port);
    /* Listen at 0.0.0.0:port */
    TcpListener(uint16_t port);

    /* Accept a new socket */
    TcpSocket * accept();
};

}

#endif // _TCPSOCKET_H_


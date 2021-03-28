#ifndef _TCPSOCKET_H_
#define _TCPSOCKET_H_

#include "Socket.h"
#include "PollObj.h"


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


#endif // _TCPSOCKET_H_


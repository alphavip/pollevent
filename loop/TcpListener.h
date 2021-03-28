#ifndef _TCPLISTENER_H_ 
#define _TCPLISTENER_H_ 

#include "TcpSocket.h"
#include "PollObj.h"

class TcpConn;

/* TCP-Listener class */
class TcpListener : public PollObj
{
public:
    TcpListener(){}
    virtual ~TcpListener(){}
    /* Listen at host:port */
    int Listen(const char *host, uint16_t port);
    /* Listen at 0.0.0.0:port */
    int Listen(uint16_t port);

    /* Accept a new socket */
    TcpConn *accept();

public:
    virtual int GetSockFd() const { return m_socket.GetFd(); };

public:
    virtual void OnData();
    virtual void OnWrite();
    virtual void OnError();

private:
    TcpSocket m_socket;
};

#endif	// _TCPLISTENER_H_
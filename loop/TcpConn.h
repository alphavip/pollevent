#ifndef _TCPTcpConn_H_ 
#define _TCPTcpConn_H_ 



#include "Buffer.h"
#include "PollObj.h"
#include "TcpSocket.h"


class TcpConn: public PollObj, public Buffer
{
public:
    TcpConn(socket_t fd) : m_socket(fd){}
    virtual ~TcpConn();
    
    // 初始化
    virtual int32_t Init();
    
    // 链接可读处理
    virtual void OnData();
    
    // 链接可写处理
    virtual void OnWrite();
    
    // 链接出错处理
    virtual void OnError();

public:
    virtual void OnConnData(){}
    virtual void OnConnError(){}

public:
    virtual int GetSockFd() const {return m_socket.GetFd(); };

public:
    void Send(uint8_t* data, uint32_t len);
    void TmpSend(BufferChain* bc);

protected:
    virtual BufferChain *Alloc();
    virtual void Cycle(BufferChain*);
    
        
protected:    
    TcpSocket m_socket; 
};

#endif // _TCPTcpConn_H_

#include "Config.h"

#include "DebugLog.h"

#include "Poll.h"

#include "TcpConn.h"


TcpConn::~TcpConn()
{
    if(this->m_pPoller != nullptr)
        m_pPoller->Remove(this);
    Clear();
}

BufferChain* TcpConn::Alloc()
{
    return this->m_pPoller->BuffPool().Get();
}

void TcpConn::Cycle(BufferChain* bc)
{
    this->m_pPoller->BuffPool().Cycle(bc);
}

int32_t TcpConn::Init()
{
	// 设置非阻塞
    if(!m_socket.makeBlock(false))
	{
        LOG("%s", "make sock unblock error!");
        return -1;
	}

    return 0;
}

void TcpConn::OnData()
{
    BufferChain* buff = this->Alloc();
    int ret = m_socket.read(buff->GetWriteAddr(), buff->GetCanWriteCount());
    if(ret < 0)
    {
        this->OnConnError();
        return;
    }
    else if(ret == 0)
    {

    }

    
    this->OnConnData();
}



void TcpConn::OnError()
{
    this->OnConnError();
}

void TcpConn::OnWrite()
{
    
}

void TcpConn::Send(uint8_t* data, uint32_t len)
{
 
}



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
    auto* bc = this->m_pPoller->BuffPool().Get();
    bc->Init();
    return bc;
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
    while (true)
    {    
        //TODO最后一次内存泄漏
        //既然用了et 这里要改 要一次性读完
        int ret = m_socket.read(buff->GetWriteAddr(), buff->GetCanWriteCount());
        if(ret < 0)
        {
            this->AddReaChain(buff);
            if(errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break;
            }
            else
            {
                std::cout << ret << ":" << errno << std::endl;
                this->OnConnError();
                return;
            }
        }
        else if(ret == 0)
        {
            this->AddReaChain(buff);
            std::cout << ret << ":" << errno << std::endl;
            this->OnConnError();
            return;
        }
        buff->write += ret;
        if(buff->GetCanWriteCount() == 0)
        {
            this->AddReaChain(buff);
            buff = this->Alloc();
        }
        std::cout << "read " << ret << std::endl;

    }

    this->TmpSend();
    this->OnConnData();
}



void TcpConn::OnError()
{
    this->OnConnError();
}

void TcpConn::OnWrite()
{
    assert(m_writeHead != nullptr);

    // 写入网络数据
#define maxionum 256
    static struct iovec iov[maxionum];
    BufferChain* pkt = m_writeHead;
    int iocount = 0;
    while(pkt != NULL)
    {
        if(iocount >= maxionum)
        {
            break;
        }
        iov[iocount].iov_base = pkt->GetReadAddr();
        iov[iocount].iov_len = pkt->GetCanReadCount();
        pkt = pkt->nextpkt;
        iocount++;
    }

    //边缘触发 这里也要改 要在这次里写完
    ssize_t wn = writev(GetSockFd(), iov, iocount);
    if(wn < 0)
    {
        return;
    }

    ssize_t count = wn;
    pkt = m_writeHead;
    while(pkt != NULL)
    {
        if(count == 0)
        {
            break;
        }

        if(count < pkt->GetCanReadCount())
        {
            pkt->read += count;
            count = 0;
            break;
        }
        else
        {
            count -= pkt->GetCanReadCount();
            m_writeHead = pkt->nextpkt;
            BufferChain* cur = pkt;
            pkt = pkt->nextpkt;
            Cycle(cur);
        }
    }


    if(m_writeHead == nullptr)
    {
        m_writeTail = nullptr;
        int32_t ret = m_pPoller->Mod(this, EV_READ|EV_ET);
        if(ret != 0)
        {
            return;
        }
    }
}

void TcpConn::Send(uint8_t* data, uint32_t len)
{

}

void TcpConn::TmpSend()
{
    // 打开epoll写入检测
    if(m_writeTail == nullptr)
    {
        int32_t ret = m_pPoller->Mod(this, EV_READ|EV_WRITE|EV_ET);
        if(ret != 0)
        {
            Clear();
            return;
        }
    }

    for(auto* bc = m_head; bc != nullptr; bc = bc->nextpkt)
    {
        this->AddWriteChain(bc);
    }

    m_head = nullptr;
    m_end = nullptr;
}


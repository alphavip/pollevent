#include "Config.h"
#include <memory.h>
#include "Buffer.h"


char* BufferChain::GetReadAddr()
{
    assert(read < _BUFFER_CHAIN_LEN);
    return this->chain + this->read;
}

char* BufferChain::GetWriteAddr()
{
    assert(write < _BUFFER_CHAIN_LEN);
    return this->chain + this->write;
}

uint16_t BufferChain::GetCanReadCount()
{
    assert(write >= read);
    return write - read;
}

uint16_t  BufferChain::GetCanWriteCount()
{
    assert(write <= _BUFFER_CHAIN_LEN);
    return _BUFFER_CHAIN_LEN - write;
}

uint32_t Buffer::CopyOut(char* out, uint32_t l)
{
    uint32_t ret = 0;
    for(BufferChain* buff = m_head; buff != nullptr; )
    {
        uint16_t canReadCount = buff->GetCanReadCount();
        if(canReadCount == 0)
        {
            BufferChain* tmp = buff->nextpkt;
            Cycle(buff);
            buff = tmp;
            m_head = buff;
            if(m_head == nullptr)
                m_end = nullptr;
        }
        else
        {
            if(l < canReadCount)
            {
                memcpy(out + ret, buff->GetReadAddr(), l);
                ret += l;
                return ret;
            }
            else
            {
                memcpy(out + ret, buff->GetReadAddr(), canReadCount);
                ret += canReadCount;
                l -= canReadCount;

                if(l == 0)
                    return ret;

                buff = buff->nextpkt;
            }
        }
    }
    return ret;
}


void Buffer::AddReaChain(BufferChain *bc)
{
    len += bc->GetCanReadCount();
    if(this->m_head == nullptr)
    {
        this->m_head = bc;
        this->m_end = bc;
    }
    else
    {
        this->m_end->nextpkt = bc;
        this->m_end = bc;
    }
}

void Buffer::AddWriteChain(BufferChain *bc)
{
    if(this->m_writeHead == nullptr)
    {
        this->m_writeHead = bc;
        this->m_writeTail = bc;
    }
    else
    {
        this->m_writeTail->nextpkt = bc;
        this->m_writeTail = bc;
    }
}

void Buffer::RemoveReadChain(uint32_t l)
{
    if(l >= len)
    {
        Clear();
        return;
    }
    uint32_t rlen = l;
    for(BufferChain* buff = this->m_head; buff != nullptr && rlen > 0;)
    {
        uint16_t canReadCount = buff->GetCanReadCount();
        if(canReadCount == 0)
        {
            BufferChain* bc = buff->nextpkt;
            Cycle(buff);
            buff = bc;
            m_head = bc;

            continue;
        }
        if(rlen < canReadCount)
        {
            buff->read += rlen;
            break;
        }
        else
        {
            rlen -= canReadCount;
            BufferChain* bc = buff->nextpkt;
            Cycle(buff);
            buff = bc;
            m_head = bc;
        }
    }
    if(m_head == nullptr)
        m_end = nullptr;

    len -= l;
}

uint32_t Buffer::CopyAndRemove(char *out, uint32_t l)
{
    uint32_t ret = 0;
    for(BufferChain* buff = this->m_head; buff != nullptr;)
    {
        uint16_t canReadCount = buff->GetCanReadCount();
        if(canReadCount == 0)
        {
            BufferChain* bc = buff->nextpkt;
            Cycle(buff);
            buff = bc;
            m_head = bc;
            continue;
        }

        if(l < canReadCount)
        {
            memcpy(out + ret, buff->GetReadAddr(), l);
            ret += l;
            len -= l;
            buff->read += l;
            break;
        }
        else
        {
            memcpy(out + ret, buff->GetReadAddr(), canReadCount);
            ret += canReadCount;
            l -= canReadCount;
            len -= canReadCount;

            BufferChain* bc = buff->nextpkt;
            Cycle(buff);
            buff = bc;
            m_head = bc;

            if(l == 0)
                break;
        }

    }

    if(m_head == nullptr)
        m_end = nullptr;

    return ret;
}


    


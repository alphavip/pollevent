#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "MemPool.h"


#define _BUFFER_CHAIN_LEN 4076

struct BufferChain
{
    char* GetReadAddr();
    char* GetWriteAddr();

    uint16_t GetCanReadCount();
    uint16_t GetCanWriteCount();

    char chain[_BUFFER_CHAIN_LEN] = {0};
    uint16_t read = 0;
    uint16_t write = 0;
    uint32_t refcount = 1;
    BufferChain* nextpkt = nullptr;

    inline void Init()
    {
        Clear();
    }
    inline void Clear()
    {
        this->read = 0;
        this->write = 0;
        this->refcount = 1;
        this->nextpkt = nullptr;
    }
};

class Buffer
{
public:
    inline void Clear()
    {
        //读buff
        for(BufferChain* tmp = m_head; tmp != nullptr;)
        {
            BufferChain* bc = tmp->nextpkt;
            assert(tmp->refcount > 0);
            if(--tmp->refcount == 0)
                Cycle(tmp);
            tmp = bc;
        }
        m_head = nullptr;
        m_end = nullptr;

        for(BufferChain* tmp = m_head; tmp != nullptr;)
        {
            BufferChain* bc = tmp->nextpkt;
            assert(tmp->refcount > 0);
            if(--tmp->refcount)
                Cycle(tmp);
        }
        m_writeHead = nullptr;
        m_writeTail = nullptr;


        len = 0;
    }

public:
    uint32_t GetLen() { return len; }
    uint32_t CopyOut(char* out, uint32_t l);

    void AddReaChain(BufferChain*);
    void RemoveReadChain(uint32_t l);

    void AddWriteChain(BufferChain* bc);


    uint32_t CopyAndRemove(char* out, uint32_t);

protected:
    virtual BufferChain *Alloc() = 0;
    virtual void Cycle(BufferChain*) = 0;

protected:
    uint32_t len = 0;
    uint32_t writelen = 0;
    BufferChain* m_head = nullptr;
    BufferChain* m_end = nullptr;
    BufferChain* m_writeHead = nullptr;
    BufferChain* m_writeTail = nullptr;
};

typedef MemPoolC<BufferChain, 4096> BufferPool;

#endif /* _BUFFER_H_ */

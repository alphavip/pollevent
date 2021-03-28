#ifndef _MEMPOOL_H_ 
#define _MEMPOOL_H_

template <typename T, int MF = 128>
class MemPoolC
{
public:
    inline MemPoolC() { objs_.fill(nullptr); }
    virtual ~MemPoolC()
    {
        for (auto *&p : objs_)
        {
            if (p != nullptr)
            {
                Free(p);
                p = nullptr;
            }
        }
    }
    inline T *Get(size_t sz = sizeof(T))
    {
        if (count_ == 0)
        {
            return Alloc(sz);
        }
        else
        {
            --count_;
            T *r = objs_[count_];
            objs_[count_] = nullptr;
            return r;
        }
    }
    inline void Cycle(T *obj)
    {
        if (count_ < MF)
        {
            objs_[count_++] = obj;
        }
        else
        {
            Free(obj);
        }
    }

private:
    static inline T *Alloc(size_t sz)
    {
        return static_cast<T *>(malloc(sz));
    }
    static inline void Free(T *obj)
    {
        free(obj);
    }

private:
    int count_ = 0;
    std::array<T *, MF> objs_;
};

#endif	// _MEMPOOL_H_
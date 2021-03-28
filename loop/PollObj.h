#ifndef _POLLOBJ_H_ 
#define _POLLOBJ_H_ 



class Poller;

class PollObj{
friend class Poller;
public:
    PollObj(){}
    virtual ~PollObj(){}

public:
    virtual int GetSockFd() const = 0;

public:
    virtual void OnData() = 0;
    virtual void OnWrite() = 0;
    virtual void OnError() = 0;

protected:
    Poller* m_pPoller = nullptr;
};

#endif // _POLLOBJ_H_
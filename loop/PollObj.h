
class PollObj{
public:
    PollObj(){}
    virtual ~PollObj(){}
public:
    virtual int GetSockFd() const = 0;

public:
    virtual void OnRead() = 0;
    virtual void OnWrite() = 0;
    virtual void OnError() = 0;
};

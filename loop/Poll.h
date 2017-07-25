#ifndef _POLL_H_
#define _POLL_H_

#define EV_READ 0x01
#define EV_WRITE 0x02
#define EV_ERROT 0x04
#define EV_PERSIST 0x08
#define EV_ET 0x10

struct EventContext;
class PollObj;

class PollEvent {
public:
    PollEvent(){}
    virtual ~PollEvent(){}

public:
    int Init(int size = 10240, int timeout = 0);
    int UnInit();

public:
    int Process();

public:
    int Add(PollObj* po, int op);
    int Mod(PollObj* po, int op);
    int Remove(PollObj* po);


private:
    EventContext* context;
};


#endif


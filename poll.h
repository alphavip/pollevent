#ifndef _POLL_H_
#define _POLL_H_

#define EV_READ 0x01
#define EV_WRITE 0x02
#define EV_PERSIST 0x04
#define EV_ET 0x08

struct EventContext;

class PollEvent {
public:
    PollEvent(){}
public:
    int Init(std::function<int (int, short)> cb, int size = 10240, int timeout = 0);
    int UnInit();
    int Process();
    int Add(int fd, int op);
    int Mod(int fd, int op);
    int Remove(int fd);


private:
    EventContext* context;
};


#endif


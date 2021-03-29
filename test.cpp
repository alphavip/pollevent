#include "Config.h"
#include "DebugLog.h"

#include <thread>

#include "TcpListener.h"
#include "Poll.h"

int callback(int fd, short op) {
    LOG("%d, %d", fd, op);
    return 0;
}

int main() {
    Poller po;
    po.Init(1024, 10);
    TcpListener tl;
    tl.Listen(8880);
    po.Add(&tl, EV_READ|EV_ET);
    while (true)
    {
        po.Process();
    }
    


    return 0;
}

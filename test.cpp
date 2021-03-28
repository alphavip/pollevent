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
    po.Init();
    TcpListener tl;
    tl.Listen(8880);
    po.Add(&tl, EV_READ);
    while (true)
    {
        po.Process();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
    


    return 0;
}

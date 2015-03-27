#include "Config.h"

#include "poll.h"
#include "debug.h"
#include "TcpSocket.h"

int callback(int fd, short op) {
    LOG("%d, %d", fd, op);
    return 0;
}

int main() {
    sys::TcpSocket ts;
    PollEvent pe;
    pe.Init();
    while(1) {
        pe.Process();
    }
    return 0;
}

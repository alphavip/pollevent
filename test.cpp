#include "Config.h"

#include "Poll.h"
#include "Debug.h"
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

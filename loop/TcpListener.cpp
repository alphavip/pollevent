#include "Config.h"

#include "Utils.h"

#include <thread>

#include "Poll.h"
#include "TcpConn.h"
#include "TcpListener.h"


int TcpListener::Listen(const char *addr, uint16_t port)
{
    while (!m_socket.bind(addr, port))
    {
        fprintf(stderr, "Unable to bind to address\n");
        fflush(stderr);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
    while (!m_socket.listen(64))
    {
        fprintf(stderr, "Unable to listen on port\n");
        fflush(stderr);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }

    fprintf(stderr, "listen on host %s:%u\n", addr, port);
    fflush(stderr);

    return 0;
}

int TcpListener::Listen(uint16_t port)
{
    while (!m_socket.bind(port))
    {
        fprintf(stderr, "Unable to bind to address\n");
        fflush(stderr);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
    while (!m_socket.listen(64))
    {
        fprintf(stderr, "Unable to listen on port\n");
        fflush(stderr);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
    fprintf(stderr, "listen on host %u\n", port);
    return 0;
}

TcpConn *TcpListener::accept()
{
    struct sockaddr_in addr = {0};
    socklen_t l = sizeof(addr);
    socket_t sock = ::accept(m_socket.GetFd(), (struct sockaddr *)&addr, &l);
    if (sock < 0)
    {
        return nullptr;
    }
    TcpConn *tc = new TcpConn(sock);
    tc->Init();
    //ts->makeKeepAlive(30, 5, 3);
    return tc;
}

void TcpListener::OnData()
{
    TcpConn* pconn = this->accept();
    if(pconn != nullptr)
    {
        this->m_pPoller->Add(pconn, EV_READ | EV_ET);
    }
}

void TcpListener::OnWrite()
{
    assert(true);
}
void TcpListener::OnError()
{
    assert(true);
}
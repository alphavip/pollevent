#include "Config.h"
#include "Utils.h"

#include "PollObj.h"
#include "Poll.h"

#ifndef EPOLLRDHUP
#define USE_OWN_RDHUP
#define EPOLLRDHUP 0x2000
#endif


struct EventContext {
    /***/
    struct epoll_event* events;
    /***/
    int nevents;
    /** the file descriptor*/
    int epollfd;
    /** wait time out*/
    int timeout;
    /** user data for this element*/
    void * data;
    /** the event for which callback was initiated */
    uint32_t cur_event;
    /** only used to enable accept and listen callbacks */
    uint8_t cb_flags;
};

int PollEvent::Init(int size, int timeout) {
    context = (struct EventContext*)malloc(sizeof(struct EventContext));
    if (context == nullptr) {
        return -1;
    }
    context->epollfd = epoll_create(32000);
    if (context->epollfd < 0) {
        free(context);
        return -1;
    }

    context->nevents = size;
    context->events = (struct epoll_event*)malloc(sizeof(struct epoll_event) * size);
    if (context->events == nullptr) {
        close(context->epollfd);
        free(context);
        return -1;
    }
    
    return 0;
}

int PollEvent::Add(PollObj* po, int op) {
    struct epoll_event ev;
    ev.data.ptr = (void*)(po);
    ev.events |= EPOLLERR | EPOLLHUP | EPOLLRDHUP;
    if(op & EV_READ)
        ev.events |= EPOLLIN;
    if(op & EV_WRITE)
        ev.events |= EPOLLOUT;
    if(op & EV_ET)
        ev.events |= EPOLLET;
    epoll_ctl(context->epollfd, EPOLL_CTL_ADD, po->GetSockFd(), &ev);
    
    return 0;
}

int PollEvent::Remove(PollObj* po) {
    epoll_ctl(context->epollfd, EPOLL_CTL_DEL, po->GetSockFd(), nullptr);
    return 0;
}



int PollEvent::Mod(PollObj* po, int op) {
    struct epoll_event ev;
    ev.data.ptr = (void*)(po);
    ev.events |= EPOLLERR | EPOLLHUP | EPOLLRDHUP;
    if(op & EV_READ)
        ev.events |= EPOLLIN;
    if(op & EV_WRITE)
        ev.events |= EPOLLOUT;
    if(op & EV_ET)
        ev.events |= EPOLLET;
    epoll_ctl(context->epollfd, EPOLL_CTL_MOD, po->GetSockFd(), &ev);
}

int PollEvent::Process() {
    int fds = epoll_wait(context->epollfd, context->events, context->nevents, context->timeout);
    for(int i = 0; i < fds; ++i) {
        auto& event = context->events[i];
        auto what = event.events;
        auto* po = (PollObj*)(event.data.ptr);
        if (what & (EPOLLHUP | EPOLLERR)) {
            po->OnError();
        }
        else {
            if(what & EPOLLIN)
                po->OnRead();
            if(what & EPOLLOUT)
                po->OnWrite();
        }
    }
    return 0;
}



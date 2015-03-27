#include "Config.h"
#include <sys/epoll.h>
#include "poll.h"


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
    std::function<int (int, short)>callback;
    /** user data for this element*/
    void * data;
    /** the event for which callback was initiated */
    uint32_t cur_event;
    /** only used to enable accept and listen callbacks */
    uint8_t cb_flags;
};

struct EventElement {
    int fd;
    epoll_event event;
    void *data;
};

int PollEvent::Init(std::function<int (int, short)>cb, int size, int timeout) {
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
    context->callback = cb;
    context->events = (struct epoll_event*)malloc(sizeof(struct epoll_event) * size);
    if (context->events == nullptr) {
        close(context->epollfd);
        free(context);
        return -1;
    }
    
    return 0;
}

int PollEvent::Add(int fd, int op) {
    EventElement* ee = (EventElement *)malloc(sizeof(EventEement));
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events |= EPOLLERR | EPOLLHUP | EPOLLRDHUP;
    if(op & EV_READ)
        ev.events |= EPOLLIN;
    if(op & EV_WRITE)
        ev.events |= EPOLLOUT;
    if(op & EV_ET)
        ev.events |= EPOLLET;
    epoll_ctl(context->epollfd, EPOLL_CTL_ADD, fd, &ev);
    
    return 0;
}

int PollEvent::Remove(int fd) {
    epoll_ctl(context->epollfd, EPOLL_CTL_DEL, fd, nullptr);
    return 0;
}



int PollEvent::Mod(int fd, int op) {
    
}

int PollEvent::Process() {
    int fds = epoll_wait(context->epollfd, context->events, context->nevents, context->timeout);
    for(int i = 0; i < fds; ++i) {
        int what = context->events[i].events;
        short ev = 0;
        if (what & (EPOLLHUP | EPOLLERR)) {
            ev = EV_READ | EV_WRITE;
        }
        else {
            if(what & EPOLLIN)
                ev |= EV_READ;
            if(what & EPOLLOUT)
                ev |= EV_WRITE;
        }
    }
    return 0;
}



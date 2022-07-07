#pragma once

// 2022/5/6
// 与epoll相关的操作

#include "utils.h"
#include <vector>

struct epoll_event;

namespace minico
{
    class Coroutine;

    class Epoller
    {
    public:
        Epoller();
        ~Epoller();

        DISALLOW_COPY_MOVE_AND_ASSIGN(Epoller);

        bool init();

        bool modifyEv(Coroutine *pCo, int fd, int interesEv);

        bool addEv(Coroutine *pCo, int fd, int interesEv);

        bool removeEv(Coroutine *pCo, int fd, int interesEv);

        int getActEvServ(int timeOutMs, std::vector<Coroutine *> &activeEvServs);

    private:
        int epollFd_;
        std::vector<struct epoll_event> activeEpollEvents_;

        inline bool isEpollFdUseful() { return epollFd_ >= 0; }
    };
};
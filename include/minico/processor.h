#pragma once

// 2022/5/11
// 线程
// 使用epoll和定时器timer进行任务调度

#include <queue>
#include <set>
#include <mutex>
#include <thread>

#include "objpool.h"
#include "spinlock.h"
#include "context.h"
#include "coroutine.h"
#include "epoller.h"
#include "timer.h"
#include "logger.h"

extern __thread int threadIdx;

namespace minico
{

    enum processerStatus
    {
        PRO_RUNNING = 0,
        PRO_STOPPING,
        PRO_STOPPED
    };

    enum newCoAddingStatus
    {
        NEWCO_ADDING = 0,
        NEWCO_ADDED
    };

    class Processor
    {
    public:
        Processor(int);
        ~Processor();

        DISALLOW_COPY_MOVE_AND_ASSIGN(Processor);

        // 运行一个新协程，该协程的函数是func
        void goNewCo(std::function<void()> &&func, size_t stackSize);
        void goNewCo(std::function<void()> &func, size_t stackSize);

        void yield();

        void wait(Time time);

        void killCurCo();

        // 线程的事件循环
        bool loop();

        void stop();

        void join();

        // 等待fd上的ev事件返回
        void waitEvent(int fd, int ev);

        inline Coroutine *getCurRunningCo() { return pCurCoroutine_; };

        inline Context *getMainCtx() { return &mainCtx_; }

        inline size_t getCoCnt() { return coSet_.size(); }

        // 运行一个指定的协程
        void goCo(Coroutine *co);
        void goCoBatch(std::vector<Coroutine *> &cos);

    private:
        // 恢复运行指定协程
        void resume(Coroutine *);
        /** 唤醒epoll */
        inline void wakeUpEpoller();

        // 当前线程Id
        int tid_;

        // 线程状态
        int status_;

        std::thread *pLoop_;

        // 协程队列
        std::queue<Coroutine *> newCoroutines_[2];

        volatile int runningNewQue_;

        Spinlock newQueLock_;

        Spinlock coPoolLock_;

        std::vector<Coroutine *> actCoroutines_;

        std::set<Coroutine *> coSet_;

        std::vector<Coroutine *> timerExpiredCo_;

        std::vector<Coroutine *> removedCo_;

        Epoller epoller_;

        Timer timer_;

        // 协程对象池
        ObjPool<Coroutine> coPool_;

        // 当前运行的协程
        Coroutine *pCurCoroutine_;

        // 上下文信息
        Context mainCtx_;
    };
};
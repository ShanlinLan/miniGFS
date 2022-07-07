#pragma once

// 2022/5/11
// 定时器，使用小根堆实现

#include <map>
#include <queue>
#include <vector>
#include <mutex>
#include <functional>

#include "mstime.h"
#include "utils.h"

#define TIMER_DUMMYBUF_SIZE 1024

namespace minico
{
    class Coroutine;
    class Epoller;

    // 定时器，timefd配合一个小根堆来实现
    // 小根堆存放的是时间和协程对象的pair
    class Timer
    {
    public:
        using TimerHeap = typename std::priority_queue<std::pair<Time, Coroutine *>, std::vector<std::pair<Time, Coroutine *>>, std::greater<std::pair<Time, Coroutine *>>>;

        Timer();
        ~Timer();

        bool init(Epoller *);

        DISALLOW_COPY_MOVE_AND_ASSIGN(Timer);

        // 获取超时的协程
        void getExpiredCoroutines(std::vector<Coroutine *> &expiredCoroutines);

        // 添加一个协程的定时器事件
        void runAt(Time time, Coroutine *pCo);

        // 添加一个协程的定时器事件
        void runAfter(Time time, Coroutine *pCo);

        // 唤醒一个协程
        void wakeUp();

    private:
        // 重新设置定时器超时时间
        bool resetTimeOfTimefd(Time time);

        inline bool isTimeFdUseful()
        {
            return timeFd_ < 0 ? false : true;
        }

        // 定时器fd
        int timeFd_;

        char dummyBuf_[TIMER_DUMMYBUF_SIZE];

        TimerHeap timerCoHeap_;
    };
};
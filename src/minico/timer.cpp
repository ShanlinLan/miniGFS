// 2022/5/11
// 定时器，使用小根堆实现

#include "../../include/minico/timer.h"
#include "../../include/minico/coroutine.h"
#include "../../include/minico/epoller.h"

#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <string.h>
#include <unistd.h>

using namespace minico;

Timer::Timer() : timeFd_(-1)
{
}

Timer::~Timer()
{
    if (isTimeFdUseful())
    {
        ::close(timeFd_);
    }
}

bool Timer::init(Epoller *pEpoller)
{
    timeFd_ = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    if (isTimeFdUseful())
    {
        return pEpoller->addEv(nullptr, timeFd_, EPOLLIN | EPOLLPRI | EPOLLRDHUP);
    }
    return false;
}

void Timer::getExpiredCoroutines(std::vector<Coroutine *> &expiredCoroutines)
{
    Time nowTime = Time::now();
    // 获取超时的协程
    while (!timerCoHeap_.empty() && timerCoHeap_.top().first <= nowTime)
    {
        expiredCoroutines.push_back(timerCoHeap_.top().second);
        timerCoHeap_.pop();
    }
    // 如果有超时协程，则读取定时器
    // 定时时间到后必须读timerd，不然会一直存在epoll事件，因为timerfd可读
    if (!expiredCoroutines.empty())
    {
        ssize_t cnt = TIMER_DUMMYBUF_SIZE;
        while (cnt >= TIMER_DUMMYBUF_SIZE)
        {
            cnt = read(timeFd_, dummyBuf_, TIMER_DUMMYBUF_SIZE);
        }
    }
    // 如果定时器中还有协程，则重新设置定时器fd的时间
    if (!timerCoHeap_.empty())
    {
        Time time = timerCoHeap_.top().first;
        resetTimeOfTimefd(time);
    }
}

bool Timer::resetTimeOfTimefd(Time time)
{
    struct itimerspec newValue;
    struct itimerspec oldValue;
    memset(&newValue, 0, sizeof newValue);
    memset(&oldValue, 0, sizeof oldValue);
    newValue.it_value = time.timeIntervalFromNow();
    int ret = timerfd_settime(timeFd_, 0, &newValue, &oldValue);
    return ret < 0 ? false : true;
}

void Timer::runAt(Time time, Coroutine* pCo)
{
	timerCoHeap_.push(std::move(std::pair<Time, Coroutine*>(time, pCo)));
	if (timerCoHeap_.top().first == time)
	{
		resetTimeOfTimefd(time);
	}
}

void Timer::runAfter(Time time, Coroutine* pCo)
{
	Time runTime(Time::now().getTimeVal() + time.getTimeVal());
	runAt(runTime, pCo);
}

// 直接将定时器设置为当前时间，则定时器超时
void Timer::wakeUp()
{
	resetTimeOfTimefd(Time::now());
}
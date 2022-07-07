// 2022/5/12
// minico协程库提供的接口

#include "../../include/minico/minico_api.h"
#include "../../include/minico/logger.h"

void minico::co_go(std::function<void()> &&func, size_t stackSize, int tid)
{
    // 由程序根据负载选择一个线程运行协程
    if (tid < 0)
    {
        LOG_INFO("CREATE A CO");
        minico::Scheduler::getScheduler()->createNewCo(std::move(func), stackSize);
    }
    else
    {
        tid %= minico::Scheduler::getScheduler()->getProCnt();
        minico::Scheduler::getScheduler()->getProcessor(tid)->goNewCo(std::move(func), stackSize);
    }
}

void minico::co_go(std::function<void()> &func, size_t stackSize, int tid)
{
    if (tid < 0)
    {
        LOG_INFO("CREATE A CO");
        minico::Scheduler::getScheduler()->createNewCo(func, stackSize);
    }
    else
    {
        tid %= minico::Scheduler::getScheduler()->getProCnt();
        minico::Scheduler::getScheduler()->getProcessor(tid)->goNewCo(func, stackSize);
    }
}

void minico::co_sleep(Time time)
{
    minico::Scheduler::getScheduler()->getProcessor(threadIdx)->wait(time);
}

void minico::sche_join()
{
    minico::Scheduler::getScheduler()->join();
}
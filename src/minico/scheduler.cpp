// 2022/5/12
// 指定协程运行在某个Processor上

#include <sys/sysinfo.h>

#include "../../include/minico/scheduler.h"

using namespace minico;

Scheduler *Scheduler::pScher_ = nullptr;
std::mutex Scheduler::scherMtx_;

Scheduler::Scheduler() : proSelector_(processors_)
{
}

Scheduler::~Scheduler()
{
    for (auto pP : processors_)
    {
        pP->stop();
    }
    for (auto pP : processors_)
    {
        pP->join();
        delete pP;
    }
}

bool Scheduler::startScheduler(int threadCnt)
{
    for (int i = 0; i < threadCnt; i++)
    {
        processors_.emplace_back(new Processor(i));
        // 开启处理器的循环
        processors_[i]->loop();
    }
    return true;
}

// 单例模式
Scheduler *Scheduler::getScheduler()
{
    if (nullptr == pScher_)
    {
        std::lock_guard<std::mutex> lock(scherMtx_);
        if (nullptr == pScher_)
        {
            pScher_ = new Scheduler();
            
            // 根据实际CPU核心数开启对应数量的线程
            pScher_->startScheduler(get_nprocs_conf());
        }
    }
    return pScher_;
}

// 找到一个Processor实例，并调用其goNewCo()函数
void Scheduler::createNewCo(std::function<void()> &&func, size_t stackSize)
{
    proSelector_.next()->goNewCo(std::move(func), stackSize);
}

void Scheduler::createNewCo(std::function<void()> &func, size_t stackSize)
{
    proSelector_.next()->goNewCo(func, stackSize);
}

void Scheduler::join()
{
    for (auto pP : processors_)
    {
        pP->join();
    }
}

Processor *Scheduler::getProcessor(int id)
{
    return processors_[id];
}

int Scheduler::getProCnt()
{
    return static_cast<int>(processors_.size());
}
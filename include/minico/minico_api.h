#pragma once

// 2022/5/12
// minico协程库提供的接口

#include "scheduler.h"
#include "mstime.h"

namespace minico
{
    void co_go(std::function<void()> &func, size_t stackSize = parameter::coroutineStackSize, int tid = -1);
    void co_go(std::function<void()> &&func, size_t stackSize = parameter::coroutineStackSize, int tid = -1);

    void co_sleep(Time t);

    void sche_join();
};
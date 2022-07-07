#pragma once

// 2022/5/6
// 环境参数设置文件

#include <stddef.h>

namespace minico
{
    namespace parameter
    {
        const static size_t coroutineStackSize = 8 * 1024;

        static constexpr int epollerListFirstSize = 16;

        static constexpr int epollTimeOutMs = -1;

        constexpr static unsigned backLog = 4096;

        static constexpr size_t memPoolMallocObjCnt = 40;
    };
};
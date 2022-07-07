#pragma once

// 2022/5/12
// 指定协程运行在某个Processor上

#include <vector>
#include <functional>

#include "processor.h"
#include "processor_selector.h"

namespace minico
{
    class Scheduler
    {
    protected:
        Scheduler();
        ~Scheduler();

    public:
        DISALLOW_COPY_MOVE_AND_ASSIGN(Scheduler);

        static Scheduler *getScheduler();

        void createNewCo(std::function<void()> &&func, size_t stackSize);
        void createNewCo(std::function<void()> &func, size_t stackSize);

        Processor *getProcessor(int);

        int getProCnt();

        void join();

    private:
        bool startScheduler(int threadCnt);
        static Scheduler *pScher_;
        static std::mutex scherMtx_;
        std::vector<Processor *> processors_;
        ProcessorSelector proSelector_;
    };

};
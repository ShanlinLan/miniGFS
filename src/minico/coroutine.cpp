// 2022/5/11
// 协程实现文件

#include "../../include/minico/coroutine.h"
#include "../../include/minico/processor.h"
#include "../../include/minico/parameter.h"

using namespace minico;

// 让当前正在运行的协程运行函数
static void coWrapFunc(Processor *pP)
{
    pP->getCurRunningCo()->startFunc();
    pP->killCurCo();
}

Coroutine::Coroutine(Processor *pMyProcessor, size_t stackSize, std::function<void()> &&func)
    : coFunc_(std::move(func)), pMyProcessor_(pMyProcessor), status_(CO_DEAD), ctx_(stackSize)
{
    status_ = CO_READY;
}

Coroutine::Coroutine(Processor *pMyProcessor, size_t stackSize, std::function<void()> &func)
    : coFunc_(std::move(func)), pMyProcessor_(pMyProcessor), status_(CO_DEAD), ctx_(stackSize)
{
    status_ = CO_READY;
}

Coroutine::~Coroutine()
{
}

void Coroutine::resume()
{
    Context *pMainCtx = pMyProcessor_->getMainCtx();
    switch (status_)
    {
    case CO_READY:
        status_ = CO_RUNNING;
        ctx_.makeContext((void (*)(void))coWrapFunc, pMyProcessor_, pMainCtx);
        ctx_.swapToMe(pMainCtx);
        break;
    case CO_WAITING:
        status_ = CO_RUNNING;
        ctx_.swapToMe(pMainCtx);
    default:
        break;
    }
}

void Coroutine::yield()
{
    status_ = CO_WAITING;
}
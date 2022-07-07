// 2022/5/6
// 封装ucontext中上下文切换的一些操作

#include <stdlib.h>

#include "../../include/minico/context.h"
#include "../../include/minico/parameter.h"

using namespace minico;

Context::Context(size_t stacksize) : pStack_(nullptr), stackSize_(stacksize) {}

Context::~Context()
{
    if (pStack_)
    {
        free(pStack_);
    }
}

// 指定对应的工作函数、处理器以及封装的上下文类，进行运行时封装
void Context::makeContext(void (*func)(), Processor *pP, Context *pLink)
{
    if (nullptr == pStack_)
    {
        pStack_ = malloc(stackSize_);
    }
    // 获取当前的寄存器信息等并保存到ctx结构体中
    getcontext(&ctx_);

    // 修改上下文信息
    ctx_.uc_stack.ss_sp = pStack_;
    ctx_.uc_stack.ss_size = parameter::coroutineStackSize;

    // 指定后继要执行的上下文，即之前保存的上下文
    ctx_.uc_link = pLink->getUCtx();

    // 开始设置上下文，1和pP是func的参数，当上下文通过setcontext或者swapcontext激活后，执行func函数
    // 当func执行返回后，继承的上下文被激活，如果继承上下文为NULL时，线程退出
    makecontext(&ctx_, func, 1, pP);
}

void Context::makeCurContext()
{
    getcontext(&ctx_);
}

// 保存当前的上下文环境到参数pOldCtx中，同时将运行上下文切换到调用该函数的Context中
void Context::swapToMe(Context *pOldCtx)
{
    // 如果为空，就利用当前ctx结构体的存储信息刷新寄存器
    if (nullptr == pOldCtx)
    {
        setcontext(&ctx_);
    }
    // 否则保存当前的上下文环境到pOldCtx的ctx结构体中
    // 并将当前调用该swapToMe函数的Context类的ctx结构体保存的上下文环境切入CPU并执行
    else
    {
        swapcontext(pOldCtx->getUCtx(), &ctx_);
    }
}
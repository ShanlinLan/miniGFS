// 2022/5/12
// 事件管理器选择器

#include "../../include/minico/processor.h"
#include "../../include/minico/processor_selector.h"

using namespace minico;

Processor *ProcessorSelector::next()
{
    int n = static_cast<int>(processors_.size());
    if (!n)
    {
        return nullptr;
    }
    int minCoProIdx = 0;
    size_t minCoCnt = processors_.front()->getCoCnt();

    switch (strategy_)
    {
    case MIN_EVENT_FIRST:
        for (int i = 1; i < n; i++)
        {
            size_t coCnt = processors_[i]->getCoCnt();
            if (coCnt < minCoCnt)
            {
                minCoCnt = coCnt;
                minCoProIdx = i;
            }
        }
        curPro_ = minCoProIdx;
        break;
    case ROUND_ROBIN:
    default:
        ++curPro_;
        if (curPro_ >= n)
        {
            curPro_ = 0;
        }
        break;
    }
    return processors_[curPro_];
}

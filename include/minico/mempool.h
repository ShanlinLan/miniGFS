#pragma once

// 2022/5/11
// 内存池，使用空闲链表实现
// 对象池创建对象时，首先会从内存池中取出相应大小的块，内存池是与对象大小强相关的，
// 其中有一个空闲链表，每次分配空间都从空闲链表上取，若空闲链表没有内容时，
// 首先会分配（40 + 分配次数）* 对象大小的空间，然后分成一个个块挂在空闲链表上，
// 这里空闲链表节点没有使用额外的空间：效仿的stl的二级配置器中的方法，
// 将数据和next指针放在了一个union中。
// 从内存池取出所需内存块后，会判断对象是否拥有non-trivial构造函数，没有的话直接返回，
// 有的话使用placement new构造对象。

#include "parameter.h"
#include "utils.h"
#include "logger.h"

namespace minico
{

    // 效仿STL二级空间配置器的实现
    struct MemBlockNode
    {
        union
        {
            MemBlockNode *next;
            char data;
        };
    };

    // 每次可以从内存池中获取objSize大小的内存块，该内存池块的大小是与对象大小强相关的
    template <size_t objSize>
    class MemPool
    {
    public:
        MemPool() : _freeListHead(nullptr), _mallocListHead(nullptr), _mallocTimes(0)
        {
            if (objSize < sizeof(MemBlockNode))
            {
                objSize_ = sizeof(MemBlockNode);
            }
            else
            {
                objSize_ = objSize;
            }
        }

        ~MemPool();

        DISALLOW_COPY_MOVE_AND_ASSIGN(MemPool);

        // 获取一个空闲的内存块
        void *AllocAMemBlock();

        // 释放一个空闲的内存块
        void FreeAMemBlock(void *block);

    private:
        // 空闲内存块链表的头节点
        MemBlockNode *_freeListHead;
        // 分配的内存链表的头节点
        MemBlockNode *_mallocListHead;
        // 分配次数
        size_t _mallocTimes;
        // 对象大小
        size_t objSize_;
    };

    template <size_t objSize>
    MemPool<objSize>::~MemPool()
    {
        while (_mallocListHead)
        {
            MemBlockNode *mallocNode = _mallocListHead;
            _mallocListHead = mallocNode->next;
            free(static_cast<void *>(mallocNode));
        }
    }

    template <size_t objSize>
    void *MemPool<objSize>::AllocAMemBlock()
    {
        void *ret;
        if (nullptr == _freeListHead)
        {
            // 首先会分配(40 + 分配次数) * 对象大小的内存空间
            size_t mallocCnt = parameter::memPoolMallocObjCnt + _mallocTimes;
            void *newMallocBlk = malloc(mallocCnt * objSize_ + sizeof(MemBlockNode));

            // 将该内存空间的首地址解释为一个MemBlockNode指针
            MemBlockNode *mallocNode = static_cast<MemBlockNode *>(newMallocBlk);

            // mallocNode是一个头节点
            mallocNode->next = _mallocListHead;
            _mallocListHead = mallocNode;

            // 将分配的内存块划分成链表节点
            newMallocBlk = static_cast<char *>(newMallocBlk) + sizeof(MemBlockNode);
            for (size_t i = 0; i < mallocCnt; ++i)
            {
                MemBlockNode *newNode = static_cast<MemBlockNode *>(newMallocBlk);
                newNode->next = _freeListHead;
                _freeListHead = newNode;
                newMallocBlk = static_cast<char *>(newMallocBlk) + objSize_;
            }

            // 分配次数加1
            ++_mallocTimes;
        }

        ret = &(_freeListHead->data);
        _freeListHead = _freeListHead->next;
        return ret;
    }

    template <size_t objSize>
    void MemPool<objSize>::FreeAMemBlock(void *block)
    {
        if (nullptr == block)
        {
            return;
        }
        MemBlockNode *newNode = static_cast<MemBlockNode *>(block);
        newNode->next = _freeListHead;
        _freeListHead = newNode;
    }
};
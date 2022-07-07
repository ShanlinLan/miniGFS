#pragma once

// 2022/5/11
// 对象池

#include <type_traits>

#include "mempool.h"

namespace minico
{
    // 对象池
    template <class T>
    class ObjPool
    {
    public:
        ObjPool(){};
        ~ObjPool(){};

        DISALLOW_COPY_MOVE_AND_ASSIGN(ObjPool);

        // 创建一个对象
        template <typename... Args>
        inline T *new_obj(Args... args);

        // 销毁一个对象
        inline void delete_obj(void *obj);

    private:
        template <typename... Args>
        inline T *new_aux(std::true_type, Args... args);

        template <typename... Args>
        inline T *new_aux(std::false_type, Args... args);

        inline void delete_aux(std::true_type, void *obj);

        inline void delete_aux(std::false_type, void *obj);

        MemPool<sizeof(T)> _memPool;
    };

    template <class T>
    template <typename... Args>
    inline T *ObjPool<T>::new_obj(Args... args)
    {
        return new_aux(std::integral_constant<bool, std::is_trivially_constructible<T>::value>(), args...);
    }

    template <class T>
    template <typename... Args>
    inline T *ObjPool<T>::new_aux(std::true_type, Args... args)
    {
        return static_cast<T *>(_memPool.AllocAMemBlock());
    }

    template <class T>
    template <typename... Args>
    inline T *ObjPool<T>::new_aux(std::false_type, Args... args)
    {
        void *newPos = _memPool.AllocAMemBlock();
        // placement new版本，它本质上是对operator new的重载，定义于#include <new>中。它不分配内存，
        //调用合适的构造函数在ptr所指的地方构造一个对象，之后返回实参指针ptr
        //调用方式 new(p)A();
        // new (p)A()调用placement new之后，还会在p上调用A::A()，这里的p可以是堆中动态分配的内存，也可以是栈中缓冲
        return new (newPos) T(args...);
    }

    template <class T>
    inline void ObjPool<T>::delete_obj(void *obj)
    {
        if (!obj)
        {
            return;
        }
        delete_aux(std::integral_constant<bool, std::is_trivially_destructible<T>::value>(), obj);
    }

    template <class T>
    inline void ObjPool<T>::delete_aux(std::true_type, void *obj)
    {
        _memPool.FreeAMemBlock(obj);
    }

    template <class T>
    inline void ObjPool<T>::delete_aux(std::false_type, void *obj)
    {
        (static_cast<T *>(obj))->~T();
        _memPool.FreeAMemBlock(obj);
    }
};
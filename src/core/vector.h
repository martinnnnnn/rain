#pragma once

#include "core/types.h"

namespace rain
{
    template<typename T>
    struct Vector
    {
        T* items;
        u32 size;
        u32 capacity;
        u32 resizeStep;
    };


    template<typename T>
    void InitVector(Vector<T>* _vector, u32 _capacity, u32 _resizeStep = 10);

    template<typename T>
    Vector<T>* CreateVector(u32 _capacity, u32 _resizeStep = 10);

    template<typename T>
    void AddItem(Vector<T>* _vector, T* _item);

    template<typename T>
    void RemoveItem(Vector<T>* _vector, T* _item);

    template<typename T>
    void RemoveItem(Vector<T>* _vector, u32 _index);

#include "vector.inl"

}
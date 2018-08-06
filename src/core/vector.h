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

    template<typename T, typename I>
    T* FindItem(Vector<T>* _vector, I _check, bool(*_compareFunction)(T, I))
    {
        T* end = _vector->items + _vector->size;
        T* iter = _vector->items;

        while (iter < end)
        {
            if (_compareFunction(*iter, _check))
            {
                return iter;
            }
            iter++;
        }
        return nullptr;
    }


    template<typename T>
    void AddItem(Vector<T>* _vector, T* _item);

    template<typename T>
    void RemoveItem(Vector<T>* _vector, T* _item);

    template<typename T>
    void RemoveItem(Vector<T>* _vector, u32 _index);

#include "vector.inl"

}
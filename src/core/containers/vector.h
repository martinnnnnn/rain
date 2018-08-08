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

        //T& operator[](u32 idx) { return items[idx]; }
        //const T& operator[](u32 idx) const { return items[idx]; }
    };

    template<typename T>
    Vector<T>* CreateVector(u32 _capacity, u32 _resizeStep);
    
    template<typename T>
    void InitVector(Vector<T>* _vector, u32 _capacity, u32 _resizeStep);
    
    template<typename T>
    void AddItem(Vector<T>* _vector, T _item);
    
    template<typename T>
    void RemoveItem(Vector<T>* _vector, T* _item);

    template<typename T>
    void RemoveItem(Vector<T>* _vector, u32 _index);
    
    template<typename T, typename I>
    T* FindItem(Vector<T>* _vector, I _check, bool(*_compareFunction)(T, I));
    
    template<typename T>
    void Resize(Vector<T>* _vector, u32 _newCapacity);
    
    template<typename T>
    void AddRangeItems(Vector<T>* _vector, T* _range, u32 _rangeCount);
    
#include "vector.inl"

}
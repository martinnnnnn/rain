#pragma once

#include "core/types.h"

namespace rain
{
    struct Vector
    {
        void** items;
        u32 size;
        u32 capacity;
        u32 resizeStep;
    };

    Vector* CreateVector(u32 _capacity, u32 _resizeStep = 10);
    void AddItem(Vector* _vector, void* _item);
    void RemoveItem(Vector* _vector, void* _item);
    void RemoveItem(Vector* _vector, u32 _index);
}
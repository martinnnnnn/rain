#pragma once

#include "core/types.h"

namespace rain::core::memory
{
    inline void* align_forward(void* _address, u8 _alignment)
    {
        return (void*)((reinterpret_cast<uptr>(_address) + static_cast<uptr>(_alignment - 1)) & static_cast<uptr>(~(_alignment - 1)));
    }

    inline u8 get_adjustment(const void* _address, u8 _alignment)
    {
        u8 adjustment = _alignment - (reinterpret_cast<uptr>(_address)& static_cast<uptr>(_alignment - 1));

        return adjustment == _alignment ? 0 : adjustment;
    }

    inline u8 get_adjustment(const void* _address, u8 _alignment, u8 _extra)
    {
        u8 adjustment = get_adjustment(_address, _alignment);

        u8 neededSpace = _extra;

        if (adjustment < neededSpace)
        {
            neededSpace -= adjustment;

            //Increase adjustment to fit header
            adjustment += _alignment * (neededSpace / _alignment);

            if (neededSpace % _alignment > 0)
                adjustment += _alignment;
        }

        return adjustment;
    }
}
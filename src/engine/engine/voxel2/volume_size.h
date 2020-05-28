#pragma once

#include "core/core.h"

namespace rain::engine::voxel2
{
    struct VolumeSize
    {
        VolumeSize(i32 size)
            : value(size)
            , squared(size * size)
            , cubed(size * size * size)
        {}

        const i32 value;
        const i32 squared;
        const i32 cubed;

        inline i32      get_offset(i32 x, i32 y, i32 z) { return x + y * value + z * squared; }
        inline i32      get_offset(const ivec3& position) { return get_offset(position.x, position.y, position.z); }
        inline ivec3    get_position(i32 offset)
        {
            const i32 z = offset / squared;
            const i32 y = (offset - z * squared) / value;
            const i32 x = (offset - z * squared - y * value);

            return ivec3{ x, y, z };
        }
    };
}



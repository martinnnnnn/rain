#pragma once

#include "core/types.h"

namespace rain::engine
{
    struct voxel_cell
    {
        enum class Type
        {
            EMPTY,
            DIRT,
            METAL,
            GLASS
        };

        Type type;
        f32 distance;
    };
}
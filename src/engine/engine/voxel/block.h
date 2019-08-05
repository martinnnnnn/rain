#pragma once

#include "core/types.h"

namespace rain::engine
{


    struct voxel_block
    {
        enum class Type
        {
            EMPTY,
            DIRT,
            METAL,
            GLASS
        };

        Type type;
        u8 health;
    };
}
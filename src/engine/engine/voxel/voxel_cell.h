#pragma once

#include "core/types.h"
#include <glm.hpp>

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
        i8 distance;
        glm::vec3 position;
    };
}
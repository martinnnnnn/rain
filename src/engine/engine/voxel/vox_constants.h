#pragma once

#include "core/core.h"

#include <glm.hpp>

namespace rain::engine::voxel
{
    using vox_position = glm::i32vec3;

    constexpr u32 BLOCK_SIZE = 16;
    constexpr u32 BLOCK_SIZE_SQUARED = BLOCK_SIZE * BLOCK_SIZE;
    constexpr u32 BLOCK_SIZE_CUBED = BLOCK_SIZE * BLOCK_SIZE * BLOCK_SIZE;

    constexpr u8 LOD_0 = 0;
    constexpr u8 LOD_1 = 1;
    constexpr u8 LOD_2 = 2;
    constexpr u8 LOD_3 = 3;
}
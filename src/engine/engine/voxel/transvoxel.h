#pragma once

#include "glm.hpp"
#include <vector>
#include <algorithm>
#include "float.h"

#include "core/core.h"
#include "voxel_cell.h"
#include "voxel_chunk.h"
#include "transvoxel_tables.h"
#include "vox_block.h"

namespace rain::engine::transvoxel
{
    constexpr u32 BLOCK_SIZE = 16;
    constexpr u32 BLOCK_SIZE_SQUARED = BLOCK_SIZE * BLOCK_SIZE;
    constexpr u32 BLOCK_SIZE_CUBED = BLOCK_SIZE * BLOCK_SIZE * BLOCK_SIZE;

    struct vertex_index
    {
        u32 cell_index;
        u32 block_index;
    };

    void transvoxel(voxel::vox_block* block, voxel::vox_cell decks[2][BLOCK_SIZE_SQUARED], u8& current_deck);
}

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

namespace rain::engine::voxel
{
    struct vertex_index
    {
        u32 cell_index;
        u32 block_index;
    };

    struct vox_cell
    {
        u8 case_code;
        vox_sample* corners[8];
        std::vector<vertex_index> indexes;
    };

    void transvoxel(vox_block* block, vox_cell decks[2][BLOCK_SIZE_SQUARED], u8& current_deck);
    void transvoxel(vox_map* map);
}

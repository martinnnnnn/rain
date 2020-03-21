#pragma once

#include "core/core.h"

#include <vector>

namespace rain::engine::voxel
{
    struct vox_sample;

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
}

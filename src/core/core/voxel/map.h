#pragma once

#include "chunk.h"
#include "stdlib.h"

namespace rain::core
{
    struct voxel_map
    {
        voxel_chunk* chunks;
        u32 chunks_count;
    };

    void init(voxel_map* vmap);
}
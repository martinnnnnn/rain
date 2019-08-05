#pragma once

#include "chunk.h"
#include "stdlib.h"

#include <vector>
#include "glm.hpp"

namespace rain::engine
{
    struct voxel_map
    {
        voxel_chunk* chunks;
        u32 chunks_count;
        u32 vao;
        std::vector<glm::mat4> model_matrices;
    };

    void init(voxel_map* vmap);
    void draw(voxel_map* vmap);
}
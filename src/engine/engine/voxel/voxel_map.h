#pragma once

#include "voxel_chunk.h"
#include "stdlib.h"

#include <vector>
#include "glm.hpp"

#include "engine/gfx/ogl/ogl_texture.h"
#include "engine/data/data_handle.h"

namespace rain::engine
{
    struct voxel_map
    {
        voxel_chunk* chunks;
        u32 chunks_count;
        u32 vao;
        std::vector<glm::mat4> model_matrices;
        handle<Texture> const * texture_handle;
    };

    void init(voxel_map* vmap);
    void draw(voxel_map* vmap);
}
#include "engine/voxel/map.h"

#include "engine/core/context.h"
#include "engine/gfx/ogl/ogl_renderer.h"


namespace rain::engine
{
    void init(voxel_map* vmap)
    {
        vmap->chunks_count = MAP_SIZE_CUBED;
        vmap->chunks = (voxel_chunk*)calloc(vmap->chunks_count, sizeof(voxel_chunk));

        for (u32 i = 0; i < MAP_SIZE; ++i)
        {
            for (u32 j = 0; j < MAP_SIZE; ++j)
            {
                for (u32 k = 0; k < MAP_SIZE; ++k)
                {
                    init(&vmap->chunks[i + j * MAP_SIZE + k * MAP_SIZE_SQUARED], vmap, uvec3{ i, j , k });
                }
            }
        }

        RAIN_LOG("%d cubes to drawn", vmap->model_matrices.size());
        RAIN_RENDERER->init_instancing(vmap->model_matrices, vmap->vao);

        for (u32 i = 0; i < MAP_SIZE; ++i)
        {
            for (u32 j = 0; j < MAP_SIZE; ++j)
            {
                for (u32 k = 0; k < MAP_SIZE; ++k)
                {
                    set_neightbours(&vmap->chunks[i + j * MAP_SIZE + k * MAP_SIZE_SQUARED]);
                }
            }
        }
    }

    void draw(voxel_map* vmap)
    {
        RAIN_RENDERER->draw_instancing(vmap->vao, vmap->model_matrices.size());
    }
}



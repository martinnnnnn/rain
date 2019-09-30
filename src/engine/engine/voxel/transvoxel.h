#pragma once

#include "glm.hpp"
#include <vector>
#include "core/core.h"
#include "voxel_cell.h"
#include "voxel_chunk.h"
#include "transvoxel_tables.h"

namespace rain::engine::isosurface
{
    constexpr u32 BLOCK_SIZE = 16;
    constexpr u32 BLOCK_SIZE_SQUARED = BLOCK_SIZE * BLOCK_SIZE;
    constexpr u32 BLOCK_SIZE_CUBED = BLOCK_SIZE * BLOCK_SIZE * BLOCK_SIZE;

    constexpr u32 TMAP_SIZE = 4;
    constexpr u32 TMAP_SIZE_SQUARED = TMAP_SIZE * TMAP_SIZE;
    constexpr u32 TMAP_SIZE_CUBED = TMAP_SIZE * TMAP_SIZE * TMAP_SIZE;

    struct tvox_point
    {
        i8 distance;

        u8 local_x;
        u8 local_y;
        u8 local_z;

        u16 world_x;
        u16 world_y;
        u16 world_z;
    };

    struct tvox_block;

    struct tvox_cell
    {
        tvox_point* corners[8];
    };

    struct tvox_map;

    struct tvox_block
    {
        tvox_point points[BLOCK_SIZE_CUBED];
        u32 x;
        u32 y;
        u32 z;
    };

    struct tvox_map
    {
        std::vector<tvox_block*> blocks;

        // temp ?
        u32 vao;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
    };

    void init_tmap(tvox_map* map);
    bool is_inside_boundary(i32 max, i32 x, i32 y, i32 z);
    tvox_block* get_tblock(tvox_map* map, i32 x, i32 y, i32 z);
    tvox_cell create_tcell(tvox_map* map, tvox_block* block, i32 x, i32 y, i32 z);
    tvox_point* get_tpoint(tvox_map* map, tvox_block* block, i32 x, i32 y, i32 z);
    void transvoxel(tvox_map* map);
    void transvoxel(tvox_map* map, tvox_block* block);

    //void transvoxel(voxel_chunk* chunk, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals);
}

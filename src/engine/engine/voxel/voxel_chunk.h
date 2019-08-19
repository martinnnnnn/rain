#pragma once

#include "core/core.h"
#include "voxel_cell.h"

#include "glm.hpp"

namespace rain::engine
{

    inline glm::u32vec3 operator*(const glm::u32vec3& v, const u32 c)
    {
        return glm::u32vec3{ v.x * c, v.y * c, v.z * c };
    }

    constexpr u32 CHUNK_SIZE = 32;
    constexpr u32 CHUNK_SIZE_SQUARED = CHUNK_SIZE * CHUNK_SIZE;
    constexpr u32 CHUNK_SIZE_CUBED = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;
    
    constexpr u32 MAP_SIZE = 7;
    constexpr u32 MAP_SIZE_SQUARED = MAP_SIZE * MAP_SIZE;
    constexpr u32 MAP_SIZE_CUBED = MAP_SIZE * MAP_SIZE * MAP_SIZE;

    constexpr u32 CHUNK_MAX_POS = CHUNK_SIZE * MAP_SIZE;
    constexpr u32 CHUNK_MAX_POS_X = CHUNK_MAX_POS;
    constexpr u32 CHUNK_MAX_POS_Y = CHUNK_MAX_POS;
    constexpr u32 CHUNK_MAX_POS_Z = CHUNK_MAX_POS;

    constexpr u32 CHUNK_NEIGHBOUR_COUNT = 6;

    constexpr u32 CHUNK_CXN = 0;
    constexpr u32 CHUNK_CXP = 1;
    constexpr u32 CHUNK_CYN = 2;
    constexpr u32 CHUNK_CYP = 3;
    constexpr u32 CHUNK_CZN = 5;
    constexpr u32 CHUNK_CZP = 6;



    struct voxel_map;

    struct voxel_chunk
    {
        voxel_cell* data;
        u32 data_size;

        voxel_map* map;
        glm::u32vec3 position;

        voxel_chunk *cXN, *cXP, *cYN, *cYP, *cZN, *cZP;

        voxel_chunk() {}
    };

    void init(voxel_chunk* chunk, voxel_map* map, const glm::u32vec3& position);
    bool is_cell_border(voxel_chunk const * const chunk, const glm::u32vec3& position);
    voxel_cell* get_cell(voxel_chunk const * const chunk, const glm::u32vec3& position);
}
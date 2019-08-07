#pragma once

#include "core/types.h"
#include "block.h"

namespace rain::engine
{
    struct uvec3
    {
        union
        {
            struct
            {
                u32 x;
                u32 y;
                u32 z;
            };
            u32 data[3];
        };
    };

    inline uvec3 operator*(const uvec3& v, const u32 c)
    {
        return uvec3{ v.x * c, v.y * c, v.z * c };
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
        voxel_block* data;
        u32 data_size;

        voxel_map* map;
        uvec3 position;

        voxel_chunk *cXN, *cXP, *cYN, *cYP, *cZN, *cZP;

        voxel_chunk() {}
    };

    void init(voxel_chunk* chunk, voxel_map* map, const uvec3& position);
    bool is_block_border(voxel_chunk const * const chunk, const uvec3& position);

}
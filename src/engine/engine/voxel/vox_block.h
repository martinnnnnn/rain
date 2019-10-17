#pragma once

#include "core/core.h"
#include "vox_sample.h"
#include "vox_constants.h"
#include "vox_cell.h"

#include <glm.hpp>
#include <vector>
#include <string>

namespace rain::engine::voxel
{
    struct vox_map;

    struct vox_vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
    };

    struct vox_block
    {
        vox_block();

        static constexpr u32 CHILD_COUNT = 8;

        vox_map* map;
        vox_sample samples[BLOCK_SIZE_CUBED];
        vox_position position;
        u8 LOD;
        vox_block* children[CHILD_COUNT];
        bool need_update;

        vox_cell last_deck[BLOCK_SIZE_SQUARED];

        u32 vao;
        u32 ebo;
        u32 vbo;
        std::vector<vox_vertex> vertices;
        std::vector<u32> indices;

    };

    void init_simplex(vox_block* block, float frequency, float amplitude, float lacunarity, float persistence);

    void free_block(vox_block* block);

    std::string save_block(vox_block* block, const std::string& file_name);
    void load_block(vox_block* block, const std::string& file_name);
    u32 encode_block(vox_block* block, u8* buffer, u32 buffer_size);
    void decode_block(vox_block* block, u8* buffer, u32 buffer_size);

    vox_position get_max_size(vox_block* block);
}
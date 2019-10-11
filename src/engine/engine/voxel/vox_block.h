#pragma once

#include "core/core.h"
#include "vox_sample.h"

#include <glm.hpp>
#include <vector>

namespace rain::engine::voxel
{
    using vox_position = glm::i32vec3;

    constexpr u32 BLOCK_SIZE = 16;
    constexpr u32 BLOCK_SIZE_SQUARED = BLOCK_SIZE * BLOCK_SIZE;
    constexpr u32 BLOCK_SIZE_CUBED = BLOCK_SIZE * BLOCK_SIZE * BLOCK_SIZE;

    struct vox_map;

    constexpr u8 LOD_0 = 0;
    constexpr u8 LOD_1 = 1;
    constexpr u8 LOD_2 = 2;
    constexpr u8 LOD_3 = 3;

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

        u32 vao;
        u32 ebo;
        u32 vbo;
        std::vector<vox_vertex> vertices;
        std::vector<u32> indices;

    };

    void init_simplex(vox_block* block, float frequency, float amplitude, float lacunarity, float persistence);

    void free_block(vox_block* block);

    void save_block(vox_block* block, const std::string& file_name);
    void load_block(vox_block* block, const std::string& file_name);
    u32 encode_block(vox_block* block, u8* buffer, u32 buffer_size);
    void decode_block(vox_block* block, u8* buffer, u32 buffer_size);

    vox_position get_max_size(vox_block* block);
    

    struct vertex_index
    {
        u32 cell_index;
        u32 block_index;
    };

    struct vox_cell
    {
        vox_cell()
            : case_code(-1)
        {
            std::fill_n(corners, 8, nullptr);
        }

        u8 case_code;
        vox_sample* corners[8];

        std::vector<vertex_index> indexes;
    };

    //transvoxel(vox_block* block);

}
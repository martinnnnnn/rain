#pragma once

#include "glm.hpp"
#include <vector>
#include <algorithm>
#include "float.h"

#include "core/core.h"
#include "voxel_cell.h"
#include "voxel_chunk.h"
#include "transvoxel_tables.h"

namespace rain::engine::transvoxel
{
    constexpr u32 BLOCK_SIZE = 16;
    constexpr u32 BLOCK_SIZE_SQUARED = BLOCK_SIZE * BLOCK_SIZE;
    constexpr u32 BLOCK_SIZE_CUBED = BLOCK_SIZE * BLOCK_SIZE * BLOCK_SIZE;

    //constexpr u32 TMAP_SIZE = 4;
    //constexpr u32 TMAP_SIZE_SQUARED = TMAP_SIZE * TMAP_SIZE;
    //constexpr u32 TMAP_SIZE_CUBED = TMAP_SIZE * TMAP_SIZE * TMAP_SIZE;

    constexpr glm::vec3 VEC3_INVALID = glm::vec3{ FLT_MAX, FLT_MAX, FLT_MAX };

    struct tvox_block;

    struct tvox_sample
    {
        i8 dist;

        u8 local_x;
        u8 local_y;
        u8 local_z;

        u16 world_x;
        u16 world_y;
        u16 world_z;

        tvox_block* owner;
    };

    struct vertex_index
    {
        u32 cell_index;
        u32 block_index;
    };

    struct tvox_cell
    {
        tvox_cell()
            : case_code(-1)
        {
            std::fill_n(corners, 8, nullptr);
        }

        u8 case_code;
        tvox_sample* corners[8];

        std::vector<vertex_index> indexes;
    };

    struct tvox_map;

    struct tvox_vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
    };

    struct tvox_block
    {
        tvox_map* map;
        tvox_sample samples[BLOCK_SIZE_CUBED];

        u32 x;
        u32 y;
        u32 z;

        u32 vao;
        u32 ebo;
        u32 vbo;
        std::vector<tvox_vertex> vertices;
        std::vector<u32> indices;

        bool need_update;
    };

    struct tvox_map
    {
        u32 xmax;
        u32 ymax;
        u32 zmax;

        glm::vec3 position;
        std::vector<tvox_block*> blocks;
    };




    void init_map(tvox_map* map, u32 xmax, u32 ymax, u32 zmax, float frequency, float amplitude, float lacunarity, float persistence);
    //void init_map(tvox_map* map);
    void transvoxel(tvox_map* map);
    void draw_map(tvox_map* map, const glm::vec3& camera_position);
    void encode_map(tvox_map* tmap, const std::string& file_path);
    void decode_map(tvox_map* tmap, const std::string& file_path);

    std::vector<tvox_sample*> get_samples_in_sphere(tvox_map* tmap, const core::sphere& sphere);

    template<typename T>
    void set_bit(T& number, T bit)
    {
        number |= 1UL << bit;
    }

    template<typename T>
    void unset_bit(T& number, T bit)
    {
        number &= ~(1UL << bit);
    }
}

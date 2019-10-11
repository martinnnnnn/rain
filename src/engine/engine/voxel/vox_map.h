#pragma once

#include "vox_block.h"

namespace rain::engine::voxel
{
    struct vox_block_file
    {
        vox_position position;
        std::string path;
        bool loaded;
    };

    struct vox_map
    {
        vox_map() : min_x(0), min_y(0), min_z(0), max_x(0), max_y(0), max_z(0), position(0.0f, 0.0f, 0.0f) {}

        i32 min_x;
        i32 min_y;
        i32 min_z;
        i32 max_x;
        i32 max_y;
        i32 max_z;
        glm::vec3 position;
        f32 max_distance;
        std::string directory_path;

        std::vector<vox_block*> blocks;
        std::vector<vox_block_file> block_paths;
    };

    void init_map(vox_map* map, const glm::vec3& position, const f32 max_distance, const std::string& directory_path);
    void init_block_paths(vox_map* map, const std::string& directory_path);

    vox_block* get_block(vox_map* map, const i32 x, const i32 y, const i32 z);
    void unload_block(vox_map* map, vox_block* block);
    void draw_map(voxel::vox_map* map, const glm::vec3& camera_position);
    void update_map(vox_map* map, const glm::vec3& position);
}
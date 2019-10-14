#include "vox_map.h"

#include "core/core.h"
#include "engine/core/context.h"
#include "engine/gfx/ogl/ogl_renderer.h"
#include "engine/core/profiler.h"
#include "voxel.h"

namespace rain::engine::voxel
{
    void init_map(vox_map* map, const glm::vec3& position, const f32 max_distance, const std::string& directory_path)
    {
        map->position = position;
        map->max_distance = max_distance;
        map->directory_path = directory_path;
        init_block_paths(map, directory_path);
    }

    void init_block_paths(vox_map* map, const std::string& directory_path)
    {
        for (const std::filesystem::directory_entry& p : std::filesystem::recursive_directory_iterator(directory_path))
        {
            if (!p.is_directory())
            {
                std::string path = p.path().string();
                core::string::replace(path, "\\", "/");
                core::file_path filepath(path);

                std::string ext = filepath.get_extention();

                if (ext.find("vox") != std::string::npos)
                {
                    std::vector<std::string> position_str = core::string::split(core::string::split(ext, ".")[0], "_");
                    vox_position position{ std::stoi(position_str[0]), std::stoi(position_str[1]), std::stoi(position_str[2]) };
                    map->block_paths.emplace_back(vox_block_file{ position, path, false });
                }
            }
        }
    }

    void update_size(vox_map* map)
    {
        if (map->blocks.size() == 0)
            return;

        i32 new_min_x = map->blocks[0]->position.x;
        i32 new_min_y = map->blocks[0]->position.y;
        i32 new_min_z = map->blocks[0]->position.z;

        i32 new_max_x = map->blocks[0]->position.x;
        i32 new_max_y = map->blocks[0]->position.y;
        i32 new_max_z = map->blocks[0]->position.z;

        for (u32 i = 0; i < map->blocks.size(); ++i)
        {
            vox_block* block = map->blocks[i];

            new_min_x = std::min(block->position.x, new_min_x);
            new_min_y = std::min(block->position.x, new_min_y);
            new_min_z = std::min(block->position.x, new_min_z);

            new_max_x = std::max(block->position.x + 1, new_max_x);
            new_max_y = std::max(block->position.x + 1, new_max_y);
            new_max_z = std::max(block->position.x + 1, new_max_z);

            //map->min_x = std::max(block->position.x, map->min_x);
            //map->min_y = std::max(block->position.y, map->min_y);
            //map->min_z = std::max(block->position.z, map->min_z);

            //map->max_x = std::min(get_max_size(block).x, map->max_x) + 1;
            //map->max_y = std::min(get_max_size(block).y, map->max_y) + 1;
            //map->max_z = std::min(get_max_size(block).z, map->max_z) + 1;
        }
        map->min_x = new_min_x;
        map->min_y = new_min_y;
        map->min_z = new_min_z;

        map->max_x = new_max_x;
        map->max_y = new_max_y;
        map->max_z = new_max_z;
    }

    void add_missing_simplex(vox_map* map)
    {
        const u32 size = (map->max_x - map->min_x) * (map->max_y - map->min_y) * (map->max_z - map->min_z);
        bool *block_presence = (bool*)calloc(size, sizeof(bool));

        for (u32 i = 0; i < map->blocks.size(); ++i)
        {
            const vox_position& position = map->blocks[i]->position - vox_position{map->min_x};
            const u32 linearized_pos = position.x + position.y * (map->max_x - map->min_x) + position.z  * (map->max_x - map->min_x)  * (map->max_y - map->min_y);
            assert(linearized_pos >= 0 && linearized_pos < size && "Wrong position computation i...");
            assert(block_presence[linearized_pos] == false && "Two block at the same place ?");
            block_presence[linearized_pos] = true;
        }

        for (i32 i = map->min_x; i < map->max_x; ++i)
        {
            for (i32 j = map->min_y; j < map->max_y; ++j)
            {
                for (i32 k = map->min_z; k < map->max_z; ++k)
                {
                    i32 iindex = i - map->min_x;
                    i32 jindex = (j - map->min_y) * (map->max_x - map->min_x);
                    i32 kindex = (k - map->min_z) * (map->max_x - map->min_x) *  (map->max_y - map->min_y);
                    i32 tindex = iindex + jindex + kindex;
                    i32 index = i + map->min_x + (j + map->min_y) * (map->max_x - map->min_x) + (k + map->min_z) * (map->max_x - map->min_x) *  (map->max_y - map->min_y);
                    if (!block_presence[tindex])
                    {
                        map->blocks.emplace_back(new vox_block());
                        vox_block* block = map->blocks.back();
                        block->map = map;
                        block->position = vox_position{ i /*+ map->min_x*/, j /*+ map->min_y*/, k /*+ map->min_z*/ };
                        init_simplex(block, 0.4f, 1.4f, 0.8f, 1.5f);
                        save_block(block, map->directory_path + "/block");
                    }
                }
            }
        }

        //for (i32 i = 0; i < map->max_x - map->min_x; ++i)
        //{
        //    for (i32 j = 0; j < map->max_y - map->min_y; ++j)
        //    {
        //        for (i32 k = 0; k < map->max_z - map->min_z; ++k)
        //        {
        //            if (!block_presence[i + map->min_x + (j + map->min_y) * (map->max_x - map->min_x) + (k + map->min_z) * (map->max_x - map->min_x) *  (map->max_y - map->min_y)])
        //            {
        //                map->blocks.emplace_back(new vox_block());
        //                vox_block* block = map->blocks.back();
        //                block->map = map;
        //                block->position = vox_position{i /*+ map->min_x*/, j /*+ map->min_y*/, k /*+ map->min_z*/ };
        //                init_simplex(block, 0.4f, 1.4f, 0.8f, 1.5f);
        //                save_block(block, map->directory_path + "/block");
        //            }
        //        }
        //    }
        //}
    }

    void unload_block(vox_map* map, vox_block* block)
    {
        u32 free_index;
        for (u32 i = 0; i < map->blocks.size(); ++i)
        {
            if (map->blocks[i] == block)
            {
                free_block(map->blocks[i]);
                delete map->blocks[i];
                free_index = i;
                break;
            }
        }

        std::swap(map->blocks.back(), map->blocks[free_index]);
        map->blocks.resize(map->blocks.size() - 1);
    }

    void update_map(vox_map* map, const glm::vec3& player_position)
    {
        RAIN_PROFILE("update_map");

        


















        for (u32 i = 0; i < map->blocks.size(); ++i)
        {
            vox_block* block = map->blocks[i];
            const glm::vec3 world_position = map->position + glm::vec3(block->position);

            if (glm::distance(player_position, world_position) > map->max_distance)
            {
                RAIN_PROFILE("save & unload");
                save_block(block, map->directory_path + "/block");
                unload_block(map, block);
            }
        }

        for (u32 i = 0; i < map->block_paths.size(); ++i)
        {
            if (map->block_paths[i].loaded)
                continue;

            const glm::vec3 world_position = map->position + glm::vec3(map->block_paths[i].position);
            if (glm::distance(player_position, world_position) < map->max_distance)
            {
                RAIN_PROFILE("load new");
                map->blocks.emplace_back(new vox_block());
                vox_block* block = map->blocks.back();
                block->map = map;
                block->position = map->block_paths[i].position;
                load_block(block, map->block_paths[i].path);
                map->block_paths[i].loaded = true;
            }
        }

        {
            RAIN_PROFILE("update size");
            update_size(map);
        }

        {
            RAIN_PROFILE("add missing");
            add_missing_simplex(map);
        }

        {
            RAIN_PROFILE("transvoxel");
            transvoxel::transvoxel(map);
        }
    }

    vox_block* get_block(vox_map* map, const i32 x, const i32 y, const i32 z)
    {
        if (core::is_inside_boundary(x, y, z, map->max_x, map->max_y, map->max_z, map->min_x, map->min_y, map->min_z))
        {
            vox_position position = vox_position{ x, y, z };
            for (u32 i = 0; i < map->blocks.size(); ++i)
            {
                if (map->blocks[i]->position == position)
                {
                    return map->blocks[i];
                }
            }
        }

        return nullptr;
    }

    void draw_map(voxel::vox_map* map, const glm::vec3& camera_position)
    {
        for (u32 i = 0; i < map->blocks.size(); ++i)
        {
            RAIN_RENDERER->draw_transvoxel(map->blocks[i]->vao, map->blocks[i]->indices.size(), camera_position, f32(BLOCK_SIZE * (map->max_y - map->min_y)));
        }
    }
}
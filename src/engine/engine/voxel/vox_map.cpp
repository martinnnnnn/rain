#include "vox_map.h"

#include "core/core.h"
#include "engine/core/context.h"
#include "engine/gfx/ogl/ogl_renderer.h"
#include "engine/core/profiler.h"
#include "voxel.h"

namespace rain::engine::voxel
{
    void init_map(vox_map* map, const glm::vec3& position, const i32 max_distance, const std::string& directory_path)
    {
        map->position = position;
        map->max_distance = max_distance;
        map->directory_path = directory_path;
        map->min_y = 0;
        map->max_y = 2;
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

    void reset_block(vox_map* map, const u32 index)
    {
        map->blocks[index].data->needs_update = true;
    }

    void unload_block(vox_map* map, const u32 index)
    {
        delete map->blocks[index].data;
        map->blocks[index].data = nullptr;
        map->blocks.erase(map->blocks.begin() + index);
    }

    void unload_blocks(vox_map* map, std::vector<u32>& indexes)
    {
        std::sort(indexes.begin(), indexes.end());

        for (auto &i = indexes.rbegin(); i != indexes.rend(); ++i)
        {
            for (u32 j = 0; j < map->block_paths.size(); ++j)
            {
                if (map->block_paths[j].position == map->blocks[*i].data->position)
                {
                    assert(map->block_paths[j].loaded && "This block should be loaded");
                    map->block_paths[j].loaded = false;
                    break;
                }
            }

            unload_block(map, *i);
        }
    }

    void update_map(vox_map* map, const glm::vec3& player_position)
    {
        const i32 player_x = std::lround(player_position.x) / i32(BLOCK_SIZE);
        const i32 player_y = std::lround(player_position.y) / i32(BLOCK_SIZE);
        const i32 player_z = std::lround(player_position.z) / i32(BLOCK_SIZE);

        const i32 last_max_x = map->max_x;
        const i32 last_max_z = map->max_z;

        map->min_x = player_x - map->max_distance;
        map->min_z = player_z - map->max_distance;

        map->max_x = player_x + map->max_distance;
        map->max_z = player_z + map->max_distance;

        // remove out of sight blocks && update blocks which have new neighbour when necessary
        for (u32 i = 0; i < map->blocks.size(); ++i)
        {
            vox_block* block = map->blocks[i].data;

            if (!core::is_inside_boundary(block->position.x, block->position.y, block->position.z, map->max_x, map->max_y, map->max_z, map->min_x, map->min_y, map->min_z))
            {
                save_block(block, map->directory_path + "/block");
                map->blocks[i].next = map->first_free;
                map->blocks[i].available = true;
                map->blocks[i].data->needs_update = true;
                map->first_free = i;
                continue;
            }

            if ((map->max_x > last_max_x && block->position.x == last_max_x - 1)
                || (map->max_z > last_max_z && block->position.z == last_max_z - 1))
            {
                block->needs_update = true;
            }
        }

        // add missing blocks
        for (i32 x = map->min_x; x < map->max_x; ++x)
        {
            for (i32 y = map->min_y; y < map->max_y; ++y) 
            {
                for (i32 z = map->min_z; z < map->max_z; ++z)
                {
                    vox_block* block = get_block(map, x, y, z);

                    if (!block)
                    {
                        bool found_in_files = false;

                        // check in files if we have the corresponding block. If so, load it
                        for (u32 i = 0; i < map->block_paths.size(); ++i)
                        {
                            if (map->block_paths[i].position == vox_position{ x, y, z })
                            {
                                assert(!map->block_paths[i].loaded && "This block should not be loaded");

                                if (map->first_free != std::numeric_limits<u32>::max())
                                {
                                    vox_map::block& bstruct = map->blocks[map->first_free];
                                    assert(bstruct.available);

                                    block = map->blocks[map->first_free].data;
                                    bstruct.available = false;
                                    map->first_free = bstruct.next;
                                    bstruct.next = std::numeric_limits<u32>::max();
                                }
                                else
                                {
                                    map->blocks.emplace_back(vox_map::block(new vox_block(map)));
                                    block = map->blocks.back().data;
                                }

                                block->map = map;
                                block->position = map->block_paths[i].position;
                                load_block(block, map->block_paths[i].path);
                                map->block_paths[i].loaded = true;
                                found_in_files = true;
                                break;
                            }
                        }

                        // if we don't find the block, we create a new one (using simplex for now).
                        if (!found_in_files)
                        {
                            if (map->first_free != std::numeric_limits<u32>::max())
                            {
                                vox_map::block& bstruct = map->blocks[map->first_free];
                                assert(bstruct.available);

                                block = map->blocks[map->first_free].data;
                                bstruct.available = false;
                                map->first_free = bstruct.next;
                                bstruct.next = std::numeric_limits<u32>::max();
                            }
                            else
                            {
                                map->blocks.emplace_back(vox_map::block(new vox_block(map)));
                                block = map->blocks.back().data;
                            }

                            block->map = map;
                            block->position = vox_position{ x, y, z };
                            init_simplex(block, 0.4f, 1.4f, 0.8f, 1.5f);
                            std::string file_path = save_block(block, map->directory_path + "/block");
                            map->block_paths.emplace_back(vox_block_file{ block->position, file_path, true });
                        }
                    }
                }
            }
        }

        transvoxel(map);
    }

    vox_block* get_block(vox_map* map, const i32 x, const i32 y, const i32 z)
    {
        if (core::is_inside_boundary(x, y, z, map->max_x, map->max_y, map->max_z, map->min_x, map->min_y, map->min_z))
        {
            vox_position position = vox_position{ x, y, z };
            for (u32 i = 0; i < map->blocks.size(); ++i)
            {
                if (map->blocks[i].data->position == position)
                {
                    return map->blocks[i].data;
                }
            }
        }

        return nullptr;
    }

    void draw_map(voxel::vox_map* map, const glm::vec3& camera_position)
    {
        //for (u32 i = 0; i < map->blocks.size(); ++i)
        //{
        //    for (u32 j = 0; j < map->blocks[i].data->vertices.size(); ++j)
        //    {
        //        RAIN_RENDERER->draw_debug_line(map->blocks[i].data->vertices[j].position, map->blocks[i].data->vertices[j].position + map->blocks[i].data->vertices[j].normal, glm::vec3(1, 0, 0));
        //    }

        //    RAIN_RENDERER->draw_transvoxel(map->blocks[i].data->vao, map->blocks[i].data->indices.size(), map->position, camera_position, f32(BLOCK_SIZE * (map->max_y - map->min_y)));
        //}
    }
}
#include "voxel_chunk.h"

#include "core/math/simplex_noise.h"
#include "engine/voxel/voxel_map.h"
#include <Windows.h>
#include "core/logger.h"
#include "engine/core/context.h"
#include "gtx/transform.hpp"

namespace rain::engine
{

    void init(voxel_chunk* chunk, voxel_map* map, const glm::u32vec3& position)
    {
        chunk->map = map;
        chunk->position = position * CHUNK_SIZE;
        chunk->data = (voxel_cell*)calloc(CHUNK_SIZE_CUBED, sizeof(voxel_cell));

        for (u32 i = 0; i < CHUNK_SIZE; ++i)
        {
            for (u32 j = 0; j < CHUNK_SIZE; ++j)
            {
                for (u32 k = 0; k < CHUNK_SIZE; ++k)
                {
                    voxel_cell* cell = &chunk->data[i + j * CHUNK_SIZE + k * CHUNK_SIZE_SQUARED];

                    //cell->distance = (core::simplex_noise::noise(float(i + chunk->position.x) / CHUNK_SIZE, float(j + chunk->position.y) / CHUNK_SIZE, float(k + chunk->position.z) / CHUNK_SIZE) + 1.0f) / 2.0f;

                    const f32 distf = core::simplex_noise::noise(float(i + chunk->position.x) / CHUNK_SIZE, float(j + chunk->position.y) / CHUNK_SIZE, float(k + chunk->position.z) / CHUNK_SIZE);
                    //RAIN_LOG("VOXEL (%f, %f, %f)", float(i + chunk->position.x) / CHUNK_SIZE, float(j + chunk->position.y) / CHUNK_SIZE, float(k + chunk->position.z) / CHUNK_SIZE);

                    cell->distance = i8(distf * 127.0f);

                    //i8 dist = i8((cell->distance - 0.5f) * 20.f);

                    cell->position = glm::vec3{ i + chunk->position.x , j + chunk->position.y , k + chunk->position.z };
                    if (cell->distance > 3)
                    {
                        cell->type = voxel_cell::Type::DIRT;
                        map->model_matrices.emplace_back(glm::translate(glm::mat4(1), cell->position));
                    }
                    else
                    {
                        cell->type = voxel_cell::Type::EMPTY;
                    }
                }
            }
        }

        const glm::u32vec3 map_index{ chunk->position.x / CHUNK_SIZE, chunk->position.y / CHUNK_SIZE, chunk->position.z / CHUNK_SIZE };

        chunk->cXN = map_index.x > 0 ? &chunk->map->chunks[(map_index.x - 1) + (map_index.y * MAP_SIZE) + (map_index.z * MAP_SIZE_SQUARED)] : nullptr;
        chunk->cYN = map_index.y > 0 ? &chunk->map->chunks[(map_index.x) + ((map_index.y - 1) * MAP_SIZE) + (map_index.z * MAP_SIZE_SQUARED)] : nullptr;
        chunk->cZN = map_index.z > 0 ? &chunk->map->chunks[(map_index.x) + ((map_index.y) * MAP_SIZE) + ((map_index.z - 1) * MAP_SIZE_SQUARED)] : nullptr;

        chunk->cXP = map_index.x < MAP_SIZE - 1 ? &chunk->map->chunks[(map_index.x + 1) + ((map_index.y) * MAP_SIZE) + ((map_index.z) * MAP_SIZE_SQUARED)] : nullptr;
        chunk->cYP = map_index.y < MAP_SIZE - 1 ? &chunk->map->chunks[(map_index.x) + ((map_index.y + 1) * MAP_SIZE) + ((map_index.z) * MAP_SIZE_SQUARED)] : nullptr;
        chunk->cZP = map_index.z < MAP_SIZE - 1 ? &chunk->map->chunks[(map_index.x) + ((map_index.y) * MAP_SIZE) + ((map_index.z + 1) * MAP_SIZE_SQUARED)] : nullptr;
    }

    voxel_cell* get_cell(voxel_chunk const * const chunk, const i32 x, const i32 y, const i32 z)
    {

        if (x < CHUNK_SIZE && y < CHUNK_SIZE && z < CHUNK_SIZE && x >= 0 && y >= 0 && z >= 0)
        {
            return  &chunk->data[x + y * CHUNK_SIZE + z * CHUNK_SIZE_SQUARED];
        }
        return nullptr;
    }


    voxel_cell* get_cell_xn(voxel_chunk const * const chunk, const u32 x, const u32 y, const u32 z)
    {
        voxel_cell* xn = nullptr;
        if (x > 0)
        {
            xn = get_cell(chunk, x - 1, y, z);
        }
        else if (chunk->cXN != nullptr)
        {
            xn = get_cell(chunk->cXN, CHUNK_SIZE - 1, y, z);
        }
        return xn;
    }

    voxel_cell* get_cell_xp(voxel_chunk const * const chunk, const u32 x, const u32 y, const u32 z)
    {
        voxel_cell* xp = nullptr;
        if (x < CHUNK_SIZE - 1)
        {
            xp = get_cell(chunk, x + 1, y, z);
        }
        else if (chunk->cXP != nullptr)
        {
            xp = get_cell(chunk->cXP, 0, y, z);
        }
        return xp;
    }

    voxel_cell* get_cell_yn(voxel_chunk const * const chunk, const u32 x, const u32 y, const u32 z)
    {
        voxel_cell* yn = nullptr;
        if (y > 0)
        {
            yn = get_cell(chunk, x - 1, y, z);
        }
        else if (chunk->cYN != nullptr)
        {
            yn = get_cell(chunk->cYN, x, CHUNK_SIZE - 1, z);
        }
        return yn;
    }

    voxel_cell* get_cell_yp(voxel_chunk const * const chunk, const u32 x, const u32 y, const u32 z)
    {
        voxel_cell* yp = nullptr;
        if (y < CHUNK_SIZE - 1)
        {
            yp = get_cell(chunk, x, y + 1, z);
        }
        else if (chunk->cYP != nullptr)
        {
            yp = get_cell(chunk->cYP, x, 0, z);
        }
        return yp;
    }

    voxel_cell* get_cell_zn(voxel_chunk const * const chunk, const u32 x, const u32 y, const u32 z)
    {
        voxel_cell* zn = nullptr;
        if (z > 0)
        {
            zn = get_cell(chunk, x, y, z - 1);
        }
        else if (chunk->cZN != nullptr)
        {
            zn = get_cell(chunk->cZN, x, y, CHUNK_SIZE - 1);
        }
        return zn;
    }

    voxel_cell* get_cell_zp(voxel_chunk const * const chunk, const u32 x, const u32 y, const u32 z)
    {
        voxel_cell* zp = nullptr;
        if (z < CHUNK_SIZE - 1)
        {
            zp = get_cell(chunk, x, y, z + 1);
        }
        else if (chunk->cZP != nullptr)
        {
            zp = get_cell(chunk->cZP, x, y, 0);
        }
        return zp;
    }

    voxel_cell* get_cell(voxel_chunk const * const chunk, const u32 x, const u32 y, const u32 z, const voxel_direction direction)
    {
        switch (direction)
        {
        case voxel_direction::XN:
            return get_cell_xn(chunk, x, y, z);
        case voxel_direction::XP:
            return get_cell_xp(chunk, x, y, z);
        case voxel_direction::YN:
            return get_cell_yn(chunk, x, y, z);
        case voxel_direction::YP:
            return get_cell_yp(chunk, x, y, z);
        case voxel_direction::ZN:
            return get_cell_zn(chunk, x, y, z);
        case voxel_direction::ZP:
            return get_cell_zp(chunk, x, y, z);
        }
        return nullptr;
    }


    bool is_cell_border(voxel_chunk const * const chunk, const glm::u32vec3& position)
    {
        voxel_cell* next = nullptr;

        
        for (u32 i = 0; i < u32(voxel_direction::COUNT); ++i)
        {
            next = get_cell(chunk, position.x, position.y, position.z, voxel_direction(i));
            if (next == nullptr || next->type == voxel_cell::Type::EMPTY)
            {
                return true;
            }
        }
        return false;




        //voxel_cell* next = nullptr;

        //// cXN
        //next = nullptr;
        //if (position.x > 0)
        //{
        //    next = get_cell(chunk, glm::u32vec3{ position.x - 1, position.y, position.z });
        //}
        //else if (chunk->cXN != nullptr)
        //{
        //    next = get_cell(chunk->cXN, glm::u32vec3{ CHUNK_SIZE - 1, position.y, position.z });
        //}
        //if (next == nullptr || next->type == voxel_cell::Type::EMPTY)
        //{
        //    return true;
        //}

        //// cXP
        //next = nullptr;
        //if (position.x < CHUNK_SIZE - 1)
        //{
        //    next = get_cell(chunk, glm::u32vec3{ position.x + 1, position.y, position.z });
        //}
        //else if (chunk->cXP != nullptr)
        //{
        //    next = get_cell(chunk->cXP, glm::u32vec3{ 0, position.y, position.z });
        //}
        //if (next == nullptr || next->type == voxel_cell::Type::EMPTY)
        //{
        //    return true;
        //}

        //------------------------
        //// cYN
        //next = nullptr;
        //if (position.y > 0)
        //{
        //    next = get_cell(chunk, glm::u32vec3{ position.x, position.y - 1, position.z });
        //}
        //else if (chunk->cYN != nullptr)
        //{
        //    next = get_cell(chunk->cYN, glm::u32vec3{ position.x, CHUNK_SIZE - 1, position.z });
        //}
        //if (next == nullptr || next->type == voxel_cell::Type::EMPTY)
        //{
        //    return true;
        //}

        //// cYP
        //next = nullptr;
        //if (position.y < CHUNK_SIZE - 1)
        //{
        //    next = get_cell(chunk, glm::u32vec3{ position.x, position.y + 1, position.z });
        //}
        //else if (chunk->cYP != nullptr)
        //{
        //    next = get_cell(chunk->cYP, glm::u32vec3{ position.x, 0, position.z });
        //}
        //if (next == nullptr || next->type == voxel_cell::Type::EMPTY)
        //{
        //    return true;
        //}
        //------------------------
        //// cZN
        //next = nullptr;
        //if (position.z > 0)
        //{
        //    next = get_cell(chunk, glm::u32vec3{ position.x, position.y, position.z - 1 });
        //}
        //else if (chunk->cZN != nullptr)
        //{
        //    next = get_cell(chunk->cZN, glm::u32vec3{ position.x, position.y, CHUNK_SIZE - 1 });
        //}
        //if (next == nullptr || next->type == voxel_cell::Type::EMPTY)
        //{
        //    return true;
        //}

        ////------------------------
        //// cZP
        //next = nullptr;
        //if (position.z < CHUNK_SIZE - 1)
        //{
        //    next = get_cell(chunk, glm::u32vec3{ position.x, position.y, position.z + 1 });
        //}
        //else if (chunk->cZP != nullptr)
        //{
        //    next = get_cell(chunk->cZP, glm::u32vec3{ position.x, position.y, 0 });
        //}
        //if (next == nullptr || next->type == voxel_cell::Type::EMPTY)
        //{
        //    return true;
        //}

        //return false;
    }
}
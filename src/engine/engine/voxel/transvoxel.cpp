#include "transvoxel.h"

#include "transvoxel_tables.h"
#include "core/core.h"
#include "engine/core/context.h"
#include "engine/core/profiler.h"
#include "engine/gfx/ogl/ogl_renderer.h"


#include "vox_map.h"
#include "vox_block.h"
#include "vox_sample.h"

#include "assert.h"
#include <algorithm>
#include <random>

namespace rain::engine::voxel
{
    void update_validity_mask(u8& validity_mask, i32 sample_x, i32 sample_y, i32 sample_z, i32 block_x, i32 block_y, i32 block_z)
    {
        core::bit_set<u8>(validity_mask, 0, sample_x);
        core::bit_set<u8>(validity_mask, 1, sample_y);
        core::bit_set<u8>(validity_mask, 2, sample_z);
    }

    void set_case_code(vox_cell* cell)
    {
        assert(cell->corners[0] && cell->corners[1] && cell->corners[2] && cell->corners[3]
            && cell->corners[4] && cell->corners[5] && cell->corners[6] && cell->corners[7] && "A cell corner can never be null.");

        cell->case_code =
            ((cell->corners[0]->dist >> 7) & 0x01)
            | ((cell->corners[1]->dist >> 6) & 0x02)
            | ((cell->corners[2]->dist >> 5) & 0x04)
            | ((cell->corners[3]->dist >> 4) & 0x08)
            | ((cell->corners[4]->dist >> 3) & 0x10)
            | ((cell->corners[5]->dist >> 2) & 0x20)
            | ((cell->corners[6]->dist >> 1) & 0x40)
            | (cell->corners[7]->dist & 0x80);
    }

    vox_sample* get_sample(vox_map* map, vox_block* block, i32 x, i32 y, i32 z)
    {
        if (core::is_inside_boundary(x, y, z, BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE))
        {
            return block->samples[x + y * BLOCK_SIZE + z * BLOCK_SIZE_SQUARED];
        }

        const i32 new_block_x = (x == -1) ? block->position.x - 1 : (x == BLOCK_SIZE) ? block->position.x + 1 : block->position.x;
        const i32 new_block_y = (y == -1) ? block->position.y - 1 : (y == BLOCK_SIZE) ? block->position.y + 1 : block->position.y;
        const i32 new_block_z = (z == -1) ? block->position.z - 1 : (z == BLOCK_SIZE) ? block->position.z + 1 : block->position.z;

        vox_block* new_block = get_block(map, new_block_x, new_block_y, new_block_z);

        if (new_block)
        {
            const i32 new_sample_x = (x == -1) ? BLOCK_SIZE - 1 : (x == BLOCK_SIZE) ? 0 : x;
            const i32 new_sample_y = (y == -1) ? BLOCK_SIZE - 1 : (y == BLOCK_SIZE) ? 0 : y;
            const i32 new_sample_z = (z == -1) ? BLOCK_SIZE - 1 : (z == BLOCK_SIZE) ? 0 : z;

            return new_block->samples[new_sample_x + new_sample_y * BLOCK_SIZE + new_sample_z * BLOCK_SIZE_SQUARED];
        }

        return nullptr;
    }

    void init_cell(vox_map* map, vox_block* block, i32 x, i32 y, i32 z, vox_cell* cell)
    {
        memset(cell, 0, sizeof(vox_cell));

        cell->corners[0] = get_sample(map, block, x    , y    , z    );
        cell->corners[1] = get_sample(map, block, x + 1, y    , z    );
        cell->corners[2] = get_sample(map, block, x    , y + 1, z    );
        cell->corners[3] = get_sample(map, block, x + 1, y + 1, z    );
        cell->corners[4] = get_sample(map, block, x    , y    , z + 1);
        cell->corners[5] = get_sample(map, block, x + 1, y    , z + 1);
        cell->corners[6] = get_sample(map, block, x    , y + 1, z + 1);
        cell->corners[7] = get_sample(map, block, x + 1, y + 1, z + 1);
    }


    void transvoxel(vox_block* block)
    {
        vox_cell decks[2][BLOCK_SIZE_SQUARED];
        u8 current_deck = 0;

        block->vertices.clear();
        block->indices.clear();

        u8 validity_mask = 0;

        i32 xmax = (block->position.x == block->map->max_x - 1) ? BLOCK_SIZE - 1 : BLOCK_SIZE;
        i32 ymax = (block->position.y == block->map->max_y - 1) ? BLOCK_SIZE - 1 : BLOCK_SIZE;
        i32 zmax = (block->position.z == block->map->max_z - 1) ? BLOCK_SIZE - 1 : BLOCK_SIZE;

        for (i32 z = 0; z < zmax; ++z)
        {
            const u8 previous_deck = (current_deck + 1) & 1;
            for (i32 y = 0; y < ymax; ++y)
            {
                for (i32 x = 0; x < xmax; ++x)
                {
                    vox_cell* cell = &decks[current_deck][x + y * BLOCK_SIZE];
                    init_cell(block->map, block, x, y, z, cell);
                    set_case_code(cell);

                    if ((cell->case_code ^ ((cell->corners[7]->dist >> 7) & 0xFF)) != 0)
                    {
                        update_validity_mask(validity_mask, x, y, z, block->position.x, block->position.y, block->position.z);

                        u8 regCellClass = lengyel::regularCellClass[cell->case_code];
                        lengyel::RegularCellData cellData = lengyel::regularCellData[regCellClass];
                        const u16* regVertexData = lengyel::regularVertexData[cell->case_code];

                        for (i32 i = 0; i < cellData.GetVertexCount(); ++i)
                        {
                            const u16 vertexData = *(regVertexData + i);

                            const u8 highByte = vertexData >> 8;    // high byte contains :
                            const u8 mapping = highByte >> 4;       // mapping to the preceeding cell
                            const u8 vertexIndex = highByte & 0x0F; // index of the vertex that needs to be reused / created

                            const u8 lowByte = vertexData & 0xFF; // low byte contains the corners between which a vertex needs to be created
                            const u8 lowNumberedCorner = lowByte >> 4;
                            const u8 highNumberedCorner = lowByte & 0x0F;


                            if ((mapping & validity_mask) == mapping)
                            {
                                i8 new_x = x - (mapping & 0x01);
                                i8 new_y = y - (((mapping & 0x02) >> 1) & 1);
                                i8 new_deck = (current_deck + ((mapping & 0x04) >> 2)) & 1;
                                
                                const vox_cell& neighbour_cell = decks[new_deck][new_x + BLOCK_SIZE * new_y];

                                for (u32 j = 0; j < neighbour_cell.indexes.size(); ++j)
                                {
                                    if (neighbour_cell.indexes[j].cell_index == vertexIndex)
                                    {
                                        cell->indexes.emplace_back(vertex_index{ 255, neighbour_cell.indexes[j].block_index });
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                const f32 t = f32(cell->corners[highNumberedCorner]->dist) / (f32(cell->corners[highNumberedCorner]->dist) - f32(cell->corners[lowNumberedCorner]->dist));

                                vox_sample* lnc_sample = cell->corners[lowNumberedCorner];
                                const f32 lnc_x = f32(i32(lnc_sample->x) + lnc_sample->owner->position.x * i32(BLOCK_SIZE));
                                const f32 lnc_y = f32(i32(lnc_sample->y) + lnc_sample->owner->position.y * i32(BLOCK_SIZE));
                                const f32 lnc_z = f32(i32(lnc_sample->z) + lnc_sample->owner->position.z * i32(BLOCK_SIZE));

                                vox_sample* hnc_sample = cell->corners[highNumberedCorner];
                                const f32 hnc_x = f32(i32(hnc_sample->x) + hnc_sample->owner->position.x * i32(BLOCK_SIZE));
                                const f32 hnc_y = f32(i32(hnc_sample->y) + hnc_sample->owner->position.y * i32(BLOCK_SIZE));
                                const f32 hnc_z = f32(i32(hnc_sample->z) + hnc_sample->owner->position.z * i32(BLOCK_SIZE));

                                if (mapping == 8)
                                {
                                    cell->indexes.emplace_back(vertex_index{ vertexIndex, block->vertices.size() });
                                }
                                else // neighbour cell not available : create & own new vertex
                                {
                                    cell->indexes.emplace_back(vertex_index{ 255, block->vertices.size() });
                                }

                                block->vertices.emplace_back(vox_vertex
                                {
                                    glm::vec3
                                    {
                                        t * lnc_x + (1 - t) * hnc_x,
                                        t * lnc_y + (1 - t) * hnc_y,
                                        t * lnc_z + (1 - t) * hnc_z
                                    },
                                    glm::vec3 {0.0f, 0.0f, 0.0f}
                                });
                            }
                        }

                        u32 block_index_0 = -1;
                        u32 block_index_1 = -1;
                        u32 block_index_2 = -1;

                        std::reverse(cellData.vertexIndex, cellData.vertexIndex + (cellData.GetTriangleCount() * 3));

                        u32 j = 0;
                        for (i32 i = 0; i < cellData.GetTriangleCount() * 3; ++i)
                        {
                            const u32 block_index = cell->indexes[cellData.vertexIndex[i]].block_index;
                            block->indices.emplace_back(block_index);
                            block_index_2 = block_index_1;
                            block_index_1 = block_index_0;
                            block_index_0 = block_index;

                            if (i % 3 == 2)
                            {
                                j++;
                                vox_vertex* A = &(block->vertices[block_index_0]);
                                vox_vertex* B = &(block->vertices[block_index_1]);
                                vox_vertex* C = &(block->vertices[block_index_2]);
                                glm::vec3 normal = glm::cross(B->position - A->position, C->position - A->position);
                                A->normal += normal;
                                B->normal += normal;
                                C->normal += normal;
                                
                            }
                        }
                    }
                }
            }
            current_deck = previous_deck;
        }

        for (u32 i = 0; i < block->vertices.size(); ++i)
        {
            block->vertices[i].normal = glm::normalize(-block->vertices[i].normal);
        }

        RAIN_RENDERER->init_transvoxel(block);

        block->needs_update = false;
    }

    void transvoxel(vox_map* map)
    {
        for (u32 i = 0; i < map->blocks.size(); ++i)
        {
            if (!map->blocks[i].data->needs_update)
                continue;

            u8 current_deck = 0;
            transvoxel(map->blocks[i].data);
        }
    }
}
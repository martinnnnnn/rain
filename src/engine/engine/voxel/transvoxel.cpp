#include "transvoxel.h"

#include "transvoxel_tables.h"
#include "core/core.h"
#include "engine/core/context.h"
#include "engine/core/profiler.h"
#include "engine/gfx/ogl/ogl_renderer.h"

#include "assert.h"
#include <algorithm>


namespace rain::engine::transvoxel
{
    void init_tblock(tvox_block* block, u32 x, u32 y, u32 z)
    {
        block->need_update = true;
        block->x = x;
        block->y = y;
        block->z = z;

        for (u32 i = 0; i < BLOCK_SIZE; ++i)
        {
            for (u32 j = 0; j < BLOCK_SIZE; ++j)
            {
                for (u32 k = 0; k < BLOCK_SIZE; ++k)
                {
                    tvox_point* point = &(block->points[i + j * BLOCK_SIZE + k * BLOCK_SIZE_SQUARED]);
                    
                    point->local_x = i;
                    point->local_y = j;
                    point->local_z = k;

                    point->world_x = i + x * BLOCK_SIZE;
                    point->world_y = j + y * BLOCK_SIZE;
                    point->world_z = k + z * BLOCK_SIZE;

                    const f32 pointx = f32(point->world_x) / f32(BLOCK_SIZE);
                    const f32 pointy = f32(point->world_y) / f32(BLOCK_SIZE);
                    const f32 pointz = f32(point->world_z) / f32(BLOCK_SIZE);

                    point->dist = i8(core::simplex_noise::noise(pointx, pointy, pointz) * 127.0f);

                    point->owner = block;
                }
            }
        }
    }

    void init_map(tvox_map* map)
    {
        map->blocks.resize(TMAP_SIZE_CUBED);

        for (u32 i = 0; i < TMAP_SIZE; ++i)
        {
            for (u32 j = 0; j < TMAP_SIZE; ++j)
            {
                for (u32 k = 0; k < TMAP_SIZE; ++k)
                {
                    u32 currentIndex = i + j * TMAP_SIZE + k * TMAP_SIZE_SQUARED;
                    map->blocks[currentIndex] = new tvox_block();
                    init_tblock(map->blocks[currentIndex], i, j, k);
                }
            }
        }
    }

    bool is_inside_boundary(i32 max, i32 x, i32 y, i32 z)
    {
        return (x < max && y < max && z < max && x >= 0 && y >= 0 && z >= 0);
    }

    tvox_block* get_block(tvox_map* map, i32 x, i32 y, i32 z)
    {
        if (is_inside_boundary(TMAP_SIZE, x, y, z))
        {
            return map->blocks[x + y * TMAP_SIZE + z * TMAP_SIZE_SQUARED];
        }

        return nullptr;
    }

    tvox_point* get_point(tvox_map* map, tvox_block* block, i32 x, i32 y, i32 z)
    {
        if (is_inside_boundary(BLOCK_SIZE, x, y, z))
        {
            return &(block->points[x + y * BLOCK_SIZE + z * BLOCK_SIZE_SQUARED]);
        }

        const i32 new_block_x = (x == -1) ? block->x - 1 : (x == BLOCK_SIZE) ? block->x + 1 : block->x;
        const i32 new_block_y = (y == -1) ? block->y - 1 : (y == BLOCK_SIZE) ? block->y + 1 : block->y;
        const i32 new_block_z = (z == -1) ? block->z - 1 : (z == BLOCK_SIZE) ? block->z + 1 : block->z;

        tvox_block* new_block = get_block(map, new_block_x, new_block_y, new_block_z);
        if (new_block)
        {
            const i32 new_cell_x = (x == -1) ? BLOCK_SIZE - 1 : (x == BLOCK_SIZE) ? 0 : x;
            const i32 new_cell_y = (y == -1) ? BLOCK_SIZE - 1 : (y == BLOCK_SIZE) ? 0 : y;
            const i32 new_cell_z = (z == -1) ? BLOCK_SIZE - 1 : (z == BLOCK_SIZE) ? 0 : z;

            return get_point(map, new_block, new_cell_x, new_cell_y, new_cell_z);
        }

        return nullptr;
    }

    void init_cell(tvox_map* map, tvox_block* block, i32 x, i32 y, i32 z, tvox_cell* cell)
    {
        memset(cell, 0, sizeof(tvox_cell));

        cell->corners[0] = get_point(map, block, x    , y    , z    );
        cell->corners[1] = get_point(map, block, x + 1, y    , z    );
        cell->corners[2] = get_point(map, block, x    , y + 1, z    );
        cell->corners[3] = get_point(map, block, x + 1, y + 1, z    );
        cell->corners[4] = get_point(map, block, x    , y    , z + 1);
        cell->corners[5] = get_point(map, block, x + 1, y    , z + 1);
        cell->corners[6] = get_point(map, block, x    , y + 1, z + 1);
        cell->corners[7] = get_point(map, block, x + 1, y + 1, z + 1);
    }

    void set_case_code(tvox_cell* cell)
    {
        assert(cell->corners[0] && "A cell corner can never be null.");
        assert(cell->corners[1] && "A cell corner can never be null.");
        assert(cell->corners[2] && "A cell corner can never be null.");
        assert(cell->corners[3] && "A cell corner can never be null.");
        assert(cell->corners[4] && "A cell corner can never be null.");
        assert(cell->corners[5] && "A cell corner can never be null.");
        assert(cell->corners[6] && "A cell corner can never be null.");
        assert(cell->corners[7] && "A cell corner can never be null.");

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

    void update_validity_mask(u8& validity_mask, i32 point_x, i32 point_y, i32 point_z, i32 block_x, i32 block_y, i32 block_z)
    {
        if (point_x == 0 /*&& block_x == 0*/)
        {
            unset_bit<u8>(validity_mask, 0);
        }
        else
        {
            set_bit<u8>(validity_mask, 0);
        }

        if (point_y == 0 /*&& block_y == 0*/)
        {
            unset_bit<u8>(validity_mask, 1);
        }
        else
        {
            set_bit<u8>(validity_mask, 1);
        }

        if (point_z == 0 /*&& block_z == 0*/)
        {
            unset_bit<u8>(validity_mask, 2);
        }
        else
        {
            set_bit<u8>(validity_mask, 2);
        }
    }

    void transvoxel(tvox_map* map, tvox_block* block, tvox_cell decks[2][BLOCK_SIZE_SQUARED], u8& current_deck)
    {
        block->vertices.clear();

        u8 validity_mask = 0;

        i32 xmax = (block->x == TMAP_SIZE - 1) ? BLOCK_SIZE - 1 : BLOCK_SIZE;
        i32 ymax = (block->y == TMAP_SIZE - 1) ? BLOCK_SIZE - 1 : BLOCK_SIZE;
        i32 zmax = (block->z == TMAP_SIZE - 1) ? BLOCK_SIZE - 1 : BLOCK_SIZE;

        for (i32 z = 0; z < zmax; ++z)
        {
            const u8 previous_deck = (current_deck + 1) & 1;
            for (i32 y = 0; y < ymax; ++y)
            {
                for (i32 x = 0; x < xmax; ++x)
                {
                    tvox_cell* cell = &decks[current_deck][x + y * BLOCK_SIZE];
                    init_cell(map, block, x, y, z, cell);
                    set_case_code(cell);

                    if ((cell->case_code ^ ((cell->corners[7]->dist >> 7) & 0xFF)) != 0)
                    {
                        update_validity_mask(validity_mask, x, y, z, block->x, block->y, block->z);

                        u8 regCellClass = lengyel::regularCellClass[cell->case_code];
                        lengyel::RegularCellData cellData = lengyel::regularCellData[regCellClass];
                        const u16* regVertexData = lengyel::regularVertexData[cell->case_code];

                        for (u32 i = 0; i < cellData.GetVertexCount(); ++i)
                        {
                            const u16 vertexData = *(regVertexData + i);

                            const u8 highByte = vertexData >> 8;    // high byte contains :
                            const u8 mapping = highByte >> 4;       // mapping to the preceeding cell
                            const u8 vertexIndex = highByte & 0x0F; // index of the vertex that needs to be reused / created

                            const u8 lowByte = vertexData & 0xFF; // low byte contains the corners between which a vertex needs to be created
                            const u8 lowNumberedCorner = lowByte >> 4;
                            const u8 highNumberedCorner = lowByte & 0x0F;


                            if (mapping & validity_mask == mapping)
                            {
                                i8 new_x = x - (mapping & 0x01);
                                i8 new_y = y - (((mapping & 0x02) >> 1) & 1);
                                i8 new_deck = (current_deck + ((mapping & 0x04) >> 2)) & 1;

                                const tvox_cell& neighbour_cell = decks[new_deck][new_x + BLOCK_SIZE * new_y];

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

                                const f32 lnc_x = f32(cell->corners[lowNumberedCorner]->world_x);
                                const f32 lnc_y = f32(cell->corners[lowNumberedCorner]->world_y);
                                const f32 lnc_z = f32(cell->corners[lowNumberedCorner]->world_z);

                                const f32 hnc_x = f32(cell->corners[highNumberedCorner]->world_x);
                                const f32 hnc_y = f32(cell->corners[highNumberedCorner]->world_y);
                                const f32 hnc_z = f32(cell->corners[highNumberedCorner]->world_z);

                                if (mapping == 8)
                                {
                                    cell->indexes.emplace_back(vertex_index{ vertexIndex, block->vertices.size() });
                                }
                                else // neighbour cell not available : create & own new vertex
                                {
                                    cell->indexes.emplace_back(vertex_index{ 255, block->vertices.size() });
                                }

                                block->vertices.emplace_back(tvox_vertex
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
                        for (i32 i = 0; i < cellData.GetTriangleCount() * 3; ++i)
                        {
                            const u32 block_index = cell->indexes[cellData.vertexIndex[i]].block_index;
                            block->indices.emplace_back(block_index);
                            block_index_2 = block_index_1;
                            block_index_1 = block_index_0;
                            block_index_0 = block_index;

                            if (i % 3 == 2)
                            {
                                tvox_vertex* A = &(block->vertices[block_index_0]);
                                tvox_vertex* B = &(block->vertices[block_index_1]);
                                tvox_vertex* C = &(block->vertices[block_index_2]);
                                glm::vec3 normal = glm::cross(B->position - A->position, C->position - A->position);
                                A->normal += normal;
                                B->normal += normal;
                                C->normal += normal;
                            }
                        }

                        //u32 j = 0;
                        //for (i32 i = cellData.GetTriangleCount() * 3 - 1; i >= 0; --i, ++j)
                        //{
                        //    const u32 block_index = cell->indexes[cellData.vertexIndex[i]].block_index;
                        //    block->indices.emplace_back(block_index);

                        //    if (j % 3 == 2)
                        //    {
                        //        tvox_vertex* A = &(block->vertices[block->indices[j]]);
                        //        tvox_vertex* B = &(block->vertices[block->indices[j - 1]]);
                        //        tvox_vertex* C = &(block->vertices[block->indices[j - 2]]);
                        //        glm::vec3 normal = glm::cross(B->position - A->position, C->position - A->position);
                        //        A->normal += normal;
                        //        B->normal += normal;
                        //        C->normal += normal;
                        //    }
                        //}
                    }
                }
            }
            current_deck = previous_deck;
        }

        for (u32 i = 0; i < block->vertices.size(); ++i)
        {
            block->vertices[i].normal = glm::normalize(block->vertices[i].normal);
        }

        if (block->vao == 0)
        {
            RAIN_RENDERER->init_transvoxel(block->vertices, block->indices, block->vao);
        }
        else
        {
            //RAIN_RENDERER->update_transvoxel(block->vertices, block->indices, block->vao);
        }

        block->need_update = false;
    }


    void transvoxel(tvox_map* map)
    {
        tvox_cell decks[2][BLOCK_SIZE_SQUARED];
        memset(decks, 0, 2 * BLOCK_SIZE_SQUARED);

        u8 current_deck = 0;

        for (u32 i = 0; i < TMAP_SIZE; ++i)
        {
            for (u32 j = 0; j < TMAP_SIZE; ++j)
            {
                for (u32 k = 0; k < TMAP_SIZE; ++k)
                {
                    tvox_block* block = get_block(map, k, j, i);

                    if (!block->need_update)
                        continue;
                    RAIN_PROFILE("Block transvoxel");
                    transvoxel(map, block, decks, current_deck);
                }
            }
        }
    }

    void draw_map(tvox_map* map, const glm::vec3& camera_position)
    {
        for (u32 i = 0; i < TMAP_SIZE; ++i)
        {
            for (u32 j = 0; j < TMAP_SIZE; ++j)
            {
                for (u32 k = 0; k < TMAP_SIZE; ++k)
                {
                    tvox_block* block = get_block(map, i, j, k);

                    RAIN_RENDERER->draw_transvoxel(block->vao, block->indices.size(), camera_position);
                }
            }
        }
    }

    std::vector<tvox_point*> get_points_in_sphere(tvox_map* tmap, const core::sphere& sphere)
    {
        std::vector<tvox_point*> points;

        for (u32 i = 0; i < TMAP_SIZE; ++i)
        {
            for (u32 j = 0; j < TMAP_SIZE; ++j)
            {
                for (u32 k = 0; k < TMAP_SIZE; ++k)
                {
                    tvox_block* block = get_block(tmap, i, j, k);

                    for (u32 x = 0; x < BLOCK_SIZE; ++x)
                    {
                        for (u32 y = 0; y < BLOCK_SIZE; ++y)
                        {
                            for (u32 z = 0; z < BLOCK_SIZE; ++z)
                            {
                                tvox_point* point = get_point(tmap, block, x, y, z);
                                const glm::vec3 position = glm::vec3(point->world_x, point->world_x, point->world_z) + tmap->position;
                                const f32 dist = glm::distance(position, sphere.offset);

                                if (dist < sphere.radius)
                                {
                                    points.push_back(point);
                                }
                            }
                        }
                    }
                }
            }
        }

        return points;
    }
}
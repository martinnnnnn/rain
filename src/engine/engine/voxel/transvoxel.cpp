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

namespace rain::engine::transvoxel
{
    void init_samples_coord(tvox_block* block)
    {
        for (u32 i = 0; i < BLOCK_SIZE; ++i)
        {
            for (u32 j = 0; j < BLOCK_SIZE; ++j)
            {
                for (u32 k = 0; k < BLOCK_SIZE; ++k)
                {
                    tvox_sample* sample = &(block->samples[i + j * BLOCK_SIZE + k * BLOCK_SIZE_SQUARED]);

                    sample->local_x = i;
                    sample->local_y = j;
                    sample->local_z = k;

                    sample->world_x = i + block->x * BLOCK_SIZE;
                    sample->world_y = j + block->y * BLOCK_SIZE;
                    sample->world_z = k + block->z * BLOCK_SIZE;
                }
            }
        }
    }

    void init_tblock(tvox_block* block, u32 x, u32 y, u32 z, float frequency, float amplitude, float lacunarity, float persistence)
    {
        u32 terraceHeight = 10;

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
                    tvox_sample* sample = &(block->samples[i + j * BLOCK_SIZE + k * BLOCK_SIZE_SQUARED]);
                    
                    sample->local_x = i;
                    sample->local_y = j;
                    sample->local_z = k;

                    sample->world_x = i + x * BLOCK_SIZE;
                    sample->world_y = j + y * BLOCK_SIZE;
                    sample->world_z = k + z * BLOCK_SIZE;

                    const f32 samplex = f32(sample->world_x) / f32(BLOCK_SIZE);
                    const f32 sampley = f32(sample->world_y) / f32(BLOCK_SIZE);
                    const f32 samplez = f32(sample->world_z) / f32(BLOCK_SIZE);

                    //core::simplex_noise n(frequency, amplitude, lacunarity, persistence);
                    //sample->dist = i8(-sample->world_y + (n.fractal(3, samplex, samplez)) * 50.0f);

                    sample->dist = i8(core::simplex_noise::noise(samplex, sampley, samplez) * 127.0f);

                    if (sample->world_x == 0 
                        || sample->world_y == 0
                        || sample->world_z == 0
                        || sample->world_x == block->map->xmax * BLOCK_SIZE - 1
                        || sample->world_y == block->map->ymax * BLOCK_SIZE - 1
                        || sample->world_z == block->map->zmax * BLOCK_SIZE - 1)
                    {
                        sample->dist = -10;
                    }

                    sample->owner = block;
                }
            }
        }
    }

    struct encoded_sample
    {
        encoded_sample(u16 c, u8 d) : count(c), dist(d) {}
        encoded_sample() : count(0), dist(0) {}

        u16 count;
        u8 dist;
    };

    struct encoded_block
    {
        std::vector<encoded_sample> samples;
        u32 x;
        u32 y;
        u32 z;
    };

    struct encoded_map
    {
        std::vector<encoded_block> blocks;
        u32 size_x;
        u32 size_y;
        u32 size_z;
    };


    void encode_map(tvox_map* tmap, const std::string& file_path)
    {
        encoded_map enc_map;
        enc_map.size_x = tmap->xmax;
        enc_map.size_y = tmap->ymax;
        enc_map.size_z = tmap->zmax;
        enc_map.blocks.resize(tmap->xmax * tmap->ymax * tmap->zmax);

        for (u32 i = 0; i < enc_map.size_x; ++i)
        {
            for (u32 j = 0; j < enc_map.size_y; ++j)
            {
                for (u32 k = 0; k < enc_map.size_z; ++k)
                {
                    u32 currentIndex = i + j * enc_map.size_x + k * enc_map.size_x * enc_map.size_y;

                    tvox_block* block = tmap->blocks[currentIndex];
                    encoded_block* enc_block = &(enc_map.blocks[currentIndex]);

                    enc_block->x = i;
                    enc_block->y = j;
                    enc_block->z = k;

                    for (u32 l = 0; l < BLOCK_SIZE_CUBED; ++l)
                    {
                        u16 count = 1;
                        while (block->samples[l].dist == block->samples[l + 1].dist)
                        {
                            count++;
                            l++;
                        }
                        enc_block->samples.emplace_back(encoded_sample(count, block->samples[l].dist));
                    }
                }
            }
        }

        const u32 size = 524'288;
        u32 counter = 0;
        u8 buffer[size];
        memset(buffer, 0, size);

        core::to_buffer(buffer, size, &counter, enc_map.size_x);
        core::to_buffer(buffer, size, &counter, enc_map.size_y);
        core::to_buffer(buffer, size, &counter, enc_map.size_z);
        core::to_buffer(buffer, size, &counter, enc_map.blocks.size());

        for (u32 i = 0; i < enc_map.blocks.size(); ++i)
        {
            encoded_block* enc_block = &enc_map.blocks[i];

            core::to_buffer(buffer, size, &counter, enc_block->x);
            core::to_buffer(buffer, size, &counter, enc_block->y);
            core::to_buffer(buffer, size, &counter, enc_block->z);
            core::to_buffer(buffer, size, &counter, enc_block->samples.size());

            for (u32 j = 0; j < enc_block->samples.size(); ++j)
            {
                core::to_buffer(buffer, size, &counter, enc_block->samples[j]);
            }
        }

        core::file::write(file_path, buffer, counter);
    }


    void decode_map(tvox_map* tmap, const std::string& file_path)
    {
        encoded_map enc_map;

        const u32 size = 524'288;
        u32 counter = 0;
        u8 buffer[size];

        u32 actual_size = core::file::read(file_path, buffer, size);

        core::from_buffer<u32>(buffer, actual_size, &counter, &enc_map.size_x);
        core::from_buffer<u32>(buffer, actual_size, &counter, &enc_map.size_y);
        core::from_buffer<u32>(buffer, actual_size, &counter, &enc_map.size_z);
        u32 blocks_size = 0;
        core::from_buffer<u32>(buffer, actual_size, &counter, &blocks_size);
        enc_map.blocks.resize(blocks_size);

        for (u32 i = 0; i < blocks_size; ++i)
        {
            encoded_block* enc_block = &(enc_map.blocks[i]);

            RAIN_PROFILE("decoding block");

            core::from_buffer(buffer, actual_size, &counter, &enc_block->x);
            core::from_buffer(buffer, actual_size, &counter, &enc_block->y);
            core::from_buffer(buffer, actual_size, &counter, &enc_block->z);
            u32 samples_size = 0;
            core::from_buffer(buffer, actual_size, &counter, &samples_size);
            enc_block->samples.resize(samples_size);

            for (u32 j = 0; j < samples_size; ++j)
            {
                core::from_buffer(buffer, actual_size, &counter, &(enc_block->samples[j]));
            }
        }

        tmap->xmax = enc_map.size_x;
        tmap->ymax = enc_map.size_y;
        tmap->zmax = enc_map.size_z;
        tmap->blocks.resize(tmap->xmax * tmap->ymax * tmap->zmax);

        for (u32 i = 0; i < tmap->xmax; ++i)
        {
            for (u32 j = 0; j < tmap->ymax; ++j)
            {
                for (u32 k = 0; k < tmap->zmax; ++k)
                {
                    u32 currentIndex = i + j * enc_map.size_x + k * enc_map.size_x * enc_map.size_y;

                    tmap->blocks[currentIndex] = new tvox_block();
                    tmap->blocks[currentIndex]->map = tmap;
                    tmap->blocks[currentIndex]->need_update = true;
                    tvox_block* block = tmap->blocks[currentIndex];
                    encoded_block* enc_block = &(enc_map.blocks[currentIndex]);


                    block->x = i;
                    block->y = j;
                    block->z = k;

                    u32 current = 0;

                    for (u32 l = 0; l < enc_block->samples.size(); ++l)
                    {
                        for (u32 count = 0; count < enc_block->samples[l].count; ++count)
                        {
                            tvox_sample* sample = &(block->samples[current]);
                            sample->dist = enc_block->samples[l].dist;
                            sample->owner = block;
                            current++;
                        }
                    }
                    init_samples_coord(block);
                }
            }
        }

    }




    void init_map(tvox_map* map, u32 xmax, u32 ymax, u32 zmax, float frequency, float amplitude, float lacunarity, float persistence)
    {
        for (u32 i = 0; i < map->blocks.size(); ++i)
        {
            delete map->blocks[i];
        }

        map->xmax = xmax;
        map->ymax = ymax;
        map->zmax = zmax;

        map->blocks.resize(xmax * ymax * zmax);

        for (u32 i = 0; i < xmax; ++i)
        {
            for (u32 j = 0; j < ymax; ++j)
            {
                for (u32 k = 0; k < zmax; ++k)
                {
                    u32 currentIndex = i + j * xmax + k * xmax * ymax;
                    RAIN_LOG("init_map : %u", currentIndex);
                    map->blocks[currentIndex] = new tvox_block();
                    map->blocks[currentIndex]->map = map;
                    init_tblock(map->blocks[currentIndex], i, j, k, frequency, amplitude, lacunarity, persistence);
                }
            }
        }
    }
    

    //void init_map(tvox_map* map)
    //{
    //    map->blocks.resize(TMAP_SIZE_CUBED);

    //    for (u32 i = 0; i < TMAP_SIZE; ++i)
    //    {
    //        for (u32 j = 0; j < TMAP_SIZE; ++j)
    //        {
    //            for (u32 k = 0; k < TMAP_SIZE; ++k)
    //            {
    //                u32 currentIndex = i + j * TMAP_SIZE + k * TMAP_SIZE_SQUARED;
    //                map->blocks[currentIndex] = new tvox_block();
    //                init_tblock(map->blocks[currentIndex], i, j, k);
    //            }
    //        }
    //    }
    //}

    tvox_block* get_block(tvox_map* map, i32 x, i32 y, i32 z)
    {
        if (core::is_inside_boundary(x, y, z, map->xmax, map->ymax, map->zmax))
        {
            return map->blocks[x + y * map->xmax + z * map->xmax * map->ymax];
        }

        return nullptr;
    }

    tvox_sample* get_sample(tvox_map* map, tvox_block* block, i32 x, i32 y, i32 z, u32 depth)
    {
        if (core::is_inside_boundary(x, y, z, BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE))
        {
            return &(block->samples[x + y * BLOCK_SIZE + z * BLOCK_SIZE_SQUARED]);
        }

        const i32 new_block_x = (x == -1) ? block->x - 1 : (x == BLOCK_SIZE) ? block->x + 1 : block->x;
        const i32 new_block_y = (y == -1) ? block->y - 1 : (y == BLOCK_SIZE) ? block->y + 1 : block->y;
        const i32 new_block_z = (z == -1) ? block->z - 1 : (z == BLOCK_SIZE) ? block->z + 1 : block->z;

        tvox_block* new_block = get_block(map, new_block_x, new_block_y, new_block_z);
        if (new_block)
        {
            const i32 new_sample_x = (x == -1) ? BLOCK_SIZE - 1 : (x == BLOCK_SIZE) ? 0 : x;
            const i32 new_sample_y = (y == -1) ? BLOCK_SIZE - 1 : (y == BLOCK_SIZE) ? 0 : y;
            const i32 new_sample_z = (z == -1) ? BLOCK_SIZE - 1 : (z == BLOCK_SIZE) ? 0 : z;

            return &(new_block->samples[new_sample_x + new_sample_y * BLOCK_SIZE + new_sample_z * BLOCK_SIZE_SQUARED]);
        }

        return nullptr;
    }

    void init_cell(tvox_map* map, tvox_block* block, i32 x, i32 y, i32 z, tvox_cell* cell)
    {
        memset(cell, 0, sizeof(tvox_cell));

        cell->corners[0] = get_sample(map, block, x    , y    , z    , 0);
        cell->corners[1] = get_sample(map, block, x + 1, y    , z    , 0);
        cell->corners[2] = get_sample(map, block, x    , y + 1, z    , 0);
        cell->corners[3] = get_sample(map, block, x + 1, y + 1, z    , 0);
        cell->corners[4] = get_sample(map, block, x    , y    , z + 1, 0);
        cell->corners[5] = get_sample(map, block, x + 1, y    , z + 1, 0);
        cell->corners[6] = get_sample(map, block, x    , y + 1, z + 1, 0);
        cell->corners[7] = get_sample(map, block, x + 1, y + 1, z + 1, 0);
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

    void update_validity_mask(u8& validity_mask, i32 sample_x, i32 sample_y, i32 sample_z, i32 block_x, i32 block_y, i32 block_z)
    {
        if (sample_x == 0 /*&& block_x == 0*/)
        {
            core::unset_bit<u8>(validity_mask, 0);
        }
        else
        {
            core::set_bit<u8>(validity_mask, 0);
        }

        if (sample_y == 0 /*&& block_y == 0*/)
        {
            core::unset_bit<u8>(validity_mask, 1);
        }
        else
        {
            core::set_bit<u8>(validity_mask, 1);
        }

        if (sample_z == 0 /*&& block_z == 0*/)
        {
            core::unset_bit<u8>(validity_mask, 2);
        }
        else
        {
            core::set_bit<u8>(validity_mask, 2);
        }
    }

    void transvoxel(tvox_map* map, tvox_block* block, tvox_cell decks[2][BLOCK_SIZE_SQUARED], u8& current_deck)
    {
        block->vertices.clear();

        u8 validity_mask = 0;

        i32 xmax = (block->x == map->xmax - 1) ? BLOCK_SIZE - 1 : BLOCK_SIZE;
        i32 ymax = (block->y == map->ymax - 1) ? BLOCK_SIZE - 1 : BLOCK_SIZE;
        i32 zmax = (block->z == map->zmax - 1) ? BLOCK_SIZE - 1 : BLOCK_SIZE;

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
            RAIN_RENDERER->init_transvoxel(block);
        }
        else
        {
            RAIN_RENDERER->update_transvoxel(block);
        }

        block->need_update = false;
    }


    void transvoxel(tvox_map* map)
    {
        tvox_cell decks[2][BLOCK_SIZE_SQUARED];
        memset(decks, 0, 2 * BLOCK_SIZE_SQUARED);

        u8 current_deck = 0;

        for (u32 i = 0; i < map->zmax; ++i)
        {
            for (u32 j = 0; j < map->ymax; ++j)
            {
                for (u32 k = 0; k < map->xmax; ++k)
                {
                    tvox_block* block = get_block(map, k, j, i);

                    if (!block->need_update)
                        continue;
                    //RAIN_PROFILE("Block transvoxel");
                    transvoxel(map, block, decks, current_deck);
                }
            }
        }
    }

    void draw_map(tvox_map* map, const glm::vec3& camera_position)
    {
        for (u32 i = 0; i < map->xmax; ++i)
        {
            for (u32 j = 0; j < map->ymax; ++j)
            {
                for (u32 k = 0; k < map->zmax; ++k)
                {
                    tvox_block* block = get_block(map, i, j, k);

                    RAIN_RENDERER->draw_transvoxel(block->vao, block->indices.size(), camera_position, f32(BLOCK_SIZE * map->ymax));
                }
            }
        }
    }



    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    void set_case_code(voxel::vox_cell* cell)
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

    voxel::vox_sample* get_sample(voxel::vox_map* map, voxel::vox_block* block, i32 x, i32 y, i32 z, u32 depth)
    {
        if (core::is_inside_boundary(x, y, z, BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE))
        {
            return &(block->samples[x + y * BLOCK_SIZE + z * BLOCK_SIZE_SQUARED]);
        }

        const i32 new_block_x = (x == -1) ? block->position.x - 1 : (x == BLOCK_SIZE) ? block->position.x + 1 : block->position.x;
        const i32 new_block_y = (y == -1) ? block->position.y - 1 : (y == BLOCK_SIZE) ? block->position.y + 1 : block->position.y;
        const i32 new_block_z = (z == -1) ? block->position.z - 1 : (z == BLOCK_SIZE) ? block->position.z + 1 : block->position.z;

        voxel::vox_block* new_block = voxel::get_block(map, new_block_x, new_block_y, new_block_z);
        if (new_block)
        {
            const i32 new_sample_x = (x == -1) ? BLOCK_SIZE - 1 : (x == BLOCK_SIZE) ? 0 : x;
            const i32 new_sample_y = (y == -1) ? BLOCK_SIZE - 1 : (y == BLOCK_SIZE) ? 0 : y;
            const i32 new_sample_z = (z == -1) ? BLOCK_SIZE - 1 : (z == BLOCK_SIZE) ? 0 : z;

            return &(new_block->samples[new_sample_x + new_sample_y * BLOCK_SIZE + new_sample_z * BLOCK_SIZE_SQUARED]);
        }

        return nullptr;
    }

    void init_cell(voxel::vox_map* map, voxel::vox_block* block, i32 x, i32 y, i32 z, voxel::vox_cell* cell)
    {
        memset(cell, 0, sizeof(voxel::vox_cell));

        cell->corners[0] = get_sample(map, block, x, y, z, 0);
        cell->corners[1] = get_sample(map, block, x + 1, y, z, 0);
        cell->corners[2] = get_sample(map, block, x, y + 1, z, 0);
        cell->corners[3] = get_sample(map, block, x + 1, y + 1, z, 0);
        cell->corners[4] = get_sample(map, block, x, y, z + 1, 0);
        cell->corners[5] = get_sample(map, block, x + 1, y, z + 1, 0);
        cell->corners[6] = get_sample(map, block, x, y + 1, z + 1, 0);
        cell->corners[7] = get_sample(map, block, x + 1, y + 1, z + 1, 0);
    }


    void transvoxel(voxel::vox_block* block, voxel::vox_cell decks[2][BLOCK_SIZE_SQUARED], u8& current_deck)
    {
        using namespace voxel;

        block->vertices.clear();

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
                                        cell->indexes.emplace_back(voxel::vertex_index{ 255, neighbour_cell.indexes[j].block_index });
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                const f32 t = f32(cell->corners[highNumberedCorner]->dist) / (f32(cell->corners[highNumberedCorner]->dist) - f32(cell->corners[lowNumberedCorner]->dist));

                                vox_sample* lnc_sample = cell->corners[lowNumberedCorner];
                                const f32 lnc_x = f32(lnc_sample->x + lnc_sample->owner->position.x * BLOCK_SIZE);
                                const f32 lnc_y = f32(lnc_sample->y + lnc_sample->owner->position.y * BLOCK_SIZE);
                                const f32 lnc_z = f32(lnc_sample->z + lnc_sample->owner->position.z * BLOCK_SIZE);

                                vox_sample* hnc_sample = cell->corners[highNumberedCorner];
                                const f32 hnc_x = f32(hnc_sample->x + hnc_sample->owner->position.x * BLOCK_SIZE);
                                const f32 hnc_y = f32(hnc_sample->y + hnc_sample->owner->position.y * BLOCK_SIZE);
                                const f32 hnc_z = f32(hnc_sample->z + hnc_sample->owner->position.z * BLOCK_SIZE);

                                if (mapping == 8)
                                {
                                    cell->indexes.emplace_back(voxel::vertex_index{ vertexIndex, block->vertices.size() });
                                }
                                else // neighbour cell not available : create & own new vertex
                                {
                                    cell->indexes.emplace_back(voxel::vertex_index{ 255, block->vertices.size() });
                                }

                                block->vertices.emplace_back(voxel::vox_vertex
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
                                voxel::vox_vertex* A = &(block->vertices[block_index_0]);
                                voxel::vox_vertex* B = &(block->vertices[block_index_1]);
                                voxel::vox_vertex* C = &(block->vertices[block_index_2]);
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
            block->vertices[i].normal = glm::normalize(block->vertices[i].normal);
        }

        RAIN_RENDERER->init_transvoxel(block);

        block->need_update = false;
    }
}
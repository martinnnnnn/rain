#include "transvoxel.h"

#include "transvoxel_tables.h"

#include "core/core.h"
#include "engine/core/context.h"

namespace rain::engine::isosurface
{
    void init_tblock(tvox_block* block, u32 x, u32 y, u32 z)
    {
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

                    point->distance = i8(core::simplex_noise::noise(pointx, pointy, pointz) * 127.0f);
                }
            }
        }
    }

    void init_tmap(tvox_map* map)
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

    tvox_block* get_tblock(tvox_map* map, i32 x, i32 y, i32 z)
    {
        if (is_inside_boundary(TMAP_SIZE, x, y, z))
        {
            return map->blocks[x + y * TMAP_SIZE + z * TMAP_SIZE_SQUARED];
        }

        return nullptr;
    }


    tvox_cell create_tcell(tvox_map* map, tvox_block* block, i32 x, i32 y, i32 z)
    {
        tvox_cell cell{};

        cell.corners[0] = get_tpoint(map, block, x - 1, y - 1, z + 1);
        cell.corners[1] = get_tpoint(map, block, x    , y - 1, z + 1);
        cell.corners[2] = get_tpoint(map, block, x - 1, y - 1, z);
        cell.corners[3] = get_tpoint(map, block, x    , y - 1, z);
        cell.corners[4] = get_tpoint(map, block, x - 1, y    , z + 1);
        cell.corners[5] = get_tpoint(map, block, x    , y    , z + 1);
        cell.corners[6] = get_tpoint(map, block, x - 1, y    , z);
        cell.corners[7] = get_tpoint(map, block, x    , y    , z);

        for (u32 i = 0; i < 8; ++i)
        {
            if (!cell.corners[i])
            {
                cell.corners[i] = cell.corners[7];
            }
        }


        return cell;
    }

    tvox_point* get_tpoint(tvox_map* map, tvox_block* block, i32 x, i32 y, i32 z)
    {
        if (is_inside_boundary(BLOCK_SIZE, x, y, z))
        {
            return &(block->points[x + y * BLOCK_SIZE + z * BLOCK_SIZE_SQUARED]);
        }

        const i32 new_block_x = (x == -1) ? block->x - 1 : (x == BLOCK_SIZE) ? block->x + 1 : block->x;
        const i32 new_block_y = (y == -1) ? block->y - 1 : (y == BLOCK_SIZE) ? block->y + 1 : block->y;
        const i32 new_block_z = (z == -1) ? block->z - 1 : (z == BLOCK_SIZE) ? block->z + 1 : block->z;

        tvox_block* new_block = get_tblock(map, new_block_x, new_block_y, new_block_z);
        if (new_block)
        {
            const i32 new_cell_x = (x == -1) ? BLOCK_SIZE - 1 : (x == BLOCK_SIZE) ? 0 : x;
            const i32 new_cell_y = (y == -1) ? BLOCK_SIZE - 1 : (y == BLOCK_SIZE) ? 0 : y;
            const i32 new_cell_z = (z == -1) ? BLOCK_SIZE - 1 : (z == BLOCK_SIZE) ? 0 : z;

            return get_tpoint(map, new_block, new_cell_x, new_cell_y, new_cell_z);
        }

        return nullptr;
    }

    void transvoxel(tvox_map* map)
    {
        for (u32 i = 0; i < TMAP_SIZE; ++i)
        {
            for (u32 j = 0; j < TMAP_SIZE; ++j)
            {
                for (u32 k = 0; k < TMAP_SIZE; ++k)
                {
                    transvoxel(map, map->blocks[i + j * TMAP_SIZE + k * TMAP_SIZE_SQUARED]);
                }
            }
        }
    }

    void transvoxel(tvox_map* map, tvox_block* block)
    {
        for (i32 z = 0; z < BLOCK_SIZE; ++z)
        {
            for (i32 y = 0; y < BLOCK_SIZE; ++y)
            {
                for (i32 x = 0; x < BLOCK_SIZE; ++x)
                {
                    tvox_cell cell = create_tcell(map, block, x, y, z);

                    unsigned long caseCode =
                        ((cell.corners[0]->distance >> 7) & 0x01)
                        | ((cell.corners[1]->distance >> 6) & 0x02)
                        | ((cell.corners[2]->distance >> 5) & 0x04)
                        | ((cell.corners[3]->distance >> 4) & 0x08)
                        | ((cell.corners[4]->distance >> 3) & 0x10)
                        | ((cell.corners[5]->distance >> 2) & 0x20)
                        | ((cell.corners[6]->distance >> 1) & 0x40)
                        | (cell.corners[7]->distance & 0x80);

                    if ((caseCode ^ ((cell.corners[7]->distance >> 7) & 0xFF)) != 0)
                    {
                        u8 regCellClass = lengyel::regularCellClass[caseCode];
                        lengyel::RegularCellData cellData = lengyel::regularCellData[regCellClass];
                        const u16* regVertexData = lengyel::regularVertexData[caseCode];

                        std::vector<glm::vec3> vertexPositions;
                        vertexPositions.resize(cellData.GetVertexCount());

                        for (u32 i = 0; i < cellData.GetVertexCount(); ++i)
                        {
                            const u16 vertexData = *(regVertexData + i);
                            const u8 lowByte = vertexData & 0xFF;
                            const u8 lowNumberedCorner = lowByte >> 4;
                            const u8 highNumberedCorner = lowByte & 0x0F;

                            const f32 t = f32(cell.corners[highNumberedCorner]->distance) / (f32(cell.corners[highNumberedCorner]->distance) - f32(cell.corners[lowNumberedCorner]->distance));

                            const f32 lnc_x = f32(cell.corners[lowNumberedCorner]->world_x);
                            const f32 lnc_y = f32(cell.corners[lowNumberedCorner]->world_y);
                            const f32 lnc_z = f32(cell.corners[lowNumberedCorner]->world_z);

                            const f32 hnc_x = f32(cell.corners[highNumberedCorner]->world_x);
                            const f32 hnc_y = f32(cell.corners[highNumberedCorner]->world_y);
                            const f32 hnc_z = f32(cell.corners[highNumberedCorner]->world_z);

                            //const f32 lnc_x = f32(cell.corners[lowNumberedCorner]->x + block->x * BLOCK_SIZE);
                            //const f32 lnc_y = f32(cell.corners[lowNumberedCorner]->y + block->y * BLOCK_SIZE);
                            //const f32 lnc_z = f32(cell.corners[lowNumberedCorner]->z + block->z * BLOCK_SIZE);

                            //const f32 hnc_x = f32(cell.corners[highNumberedCorner]->x + block->x * BLOCK_SIZE);
                            //const f32 hnc_y = f32(cell.corners[highNumberedCorner]->y + block->y * BLOCK_SIZE);
                            //const f32 hnc_z = f32(cell.corners[highNumberedCorner]->z + block->z * BLOCK_SIZE);

                            vertexPositions[i] = glm::vec3
                            {
                                t * lnc_x + (1 - t) * hnc_x,
                                t * lnc_y + (1 - t) * hnc_y,
                                t * lnc_z + (1 - t) * hnc_z
                            };
                        }

                        u32 j = 0;
                        for (i32 i = cellData.GetTriangleCount() * 3 - 1; i >= 0; --i, ++j)
                        {
                            map->vertices.push_back(vertexPositions[cellData.vertexIndex[i]]);
                            if (j % 3 == 0)
                            {
                                const glm::vec3& A = vertexPositions[cellData.vertexIndex[i]];
                                const glm::vec3& B = vertexPositions[cellData.vertexIndex[i - 1]];
                                const glm::vec3& C = vertexPositions[cellData.vertexIndex[i - 2]];
                                map->normals.push_back(glm::normalize(glm::cross(B - A, C - A)));
                                map->normals.push_back(glm::normalize(glm::cross(B - A, C - A)));
                                map->normals.push_back(glm::normalize(glm::cross(B - A, C - A)));
                            }
                        }
                    }
                }
            }
        }
    }

    //void transvoxel(voxel_chunk* chunk, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals)
    //{
    //    for (i32 z = 0; z < CHUNK_SIZE; ++z)
    //    {
    //        for (i32 y = 0; y < CHUNK_SIZE; ++y)
    //        {
    //            for (i32 x = 0; x < CHUNK_SIZE; ++x)
    //            {
    //                voxel_cell* cells[8];
    //                cells[0] = get_cell(chunk, x - 1, y - 1, z + 1);
    //                cells[1] = get_cell(chunk, x    , y - 1, z + 1);
    //                cells[2] = get_cell(chunk, x - 1, y - 1, z    );
    //                cells[3] = get_cell(chunk, x    , y - 1, z    );
    //                cells[4] = get_cell(chunk, x - 1, y    , z + 1);
    //                cells[5] = get_cell(chunk, x    , y    , z + 1);
    //                cells[6] = get_cell(chunk, x - 1, y    , z    );
    //                cells[7] = get_cell(chunk, x    , y    , z    );

    //                for (u32 i = 0; i < 8; ++i)
    //                {
    //                    if (!cells[i])
    //                    {
    //                        cells[i] = cells[7];
    //                    }
    //                }

    //                unsigned long caseCode = 
    //                      ((cells[0]->distance >> 7) & 0x01) 
    //                    | ((cells[1]->distance >> 6) & 0x02) 
    //                    | ((cells[2]->distance >> 5) & 0x04) 
    //                    | ((cells[3]->distance >> 4) & 0x08) 
    //                    | ((cells[4]->distance >> 3) & 0x10)
    //                    | ((cells[5]->distance >> 2) & 0x20) 
    //                    | ((cells[6]->distance >> 1) & 0x40) 
    //                    | (cells[7]->distance & 0x80);
    //                
    //                if ((caseCode ^ ((cells[7]->distance >> 7) & 0xFF)) != 0)
    //                {
    //                    u8 regCellClass = lengyel::regularCellClass[caseCode];
    //                    lengyel::RegularCellData cellData = lengyel::regularCellData[regCellClass];
    //                    const u16* regVertexData = lengyel::regularVertexData[caseCode];

    //                    std::vector<glm::vec3> vertexPositions;
    //                    vertexPositions.resize(cellData.GetVertexCount());

    //                    for (u32 i = 0; i < cellData.GetVertexCount(); ++i)
    //                    {
    //                        const u16 vertexData = *(regVertexData + i);
    //                        const u8 lowByte = vertexData & 0xFF;
    //                        const u8 lowNumberedCorner = lowByte >> 4;
    //                        const u8 highNumberedCorner = lowByte & 0x0F;

    //                        const f32 t = f32(cells[highNumberedCorner]->distance) / (f32(cells[highNumberedCorner]->distance) - f32(cells[lowNumberedCorner]->distance));
    //                        vertexPositions[i] = t * cells[lowNumberedCorner]->position + (1 - t) * cells[highNumberedCorner]->position;
    //                    }

    //                    u32 j = 0;
    //                    for (i32 i = cellData.GetTriangleCount() * 3 - 1; i >= 0; --i, ++j)
    //                    {
    //                        vertices.push_back(vertexPositions[cellData.vertexIndex[i]]);
    //                        if (j % 3 == 0)
    //                        {
    //                            const glm::vec3& A = vertexPositions[cellData.vertexIndex[i    ]];
    //                            const glm::vec3& B = vertexPositions[cellData.vertexIndex[i - 1]];
    //                            const glm::vec3& C = vertexPositions[cellData.vertexIndex[i - 2]];
    //                            normals.push_back(glm::normalize(glm::cross(B - A, C - A)));
    //                            normals.push_back(glm::normalize(glm::cross(B - A, C - A)));
    //                            normals.push_back(glm::normalize(glm::cross(B - A, C - A)));
    //                        }
    //                    }
    //                }
    //            }
    //        }
    //    }
    //}
 
}
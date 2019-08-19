#include "transvoxel.h"

#include "transvoxel_tables.h"


namespace rain::engine::isosurface
{
    glm::i32vec3 corner_index[] =
    {
        glm::i32vec3{ 0, 0, 0 },
        glm::i32vec3{ 1, 0, 0 },
        glm::i32vec3{ 0, 0, 1 },
        glm::i32vec3{ 1, 0, 1 },
        glm::i32vec3{ 0, 1, 0 },
        glm::i32vec3{ 1, 1, 0 },
        glm::i32vec3{ 0, 1, 1 },
        glm::i32vec3{ 1, 1, 1 }
    };


    u8 getCaseCode(i8 density[8])
    {
        u8 code = 0;
        u8 konj = 0x01;
        for (u32 i = 0; i < 8; i++)
        {
            code |= (u8)((density[i] >> (8 - 1 - i)) & konj);
            konj <<= 1;
        }

        return code;
    }


    glm::vec3 interpolate_voxel_vector(long t, const glm::vec3& P0, const glm::vec3& P1)
    {
        long u = 0x0100 - t; //256 - t
        float s = 1.0f / 256.0f;
        glm::vec3 Q = P0 * t + P1 * u; //Density Interpolation
        Q *= s; // shift to shader ! 
        return Q;
    }


    void generate_vertex(glm::i32vec3& offsetPos, voxel_mesh* mesh, int LOD, long t, u8& v0, u8& v1, i8& d0, i8& d1, const glm::vec3& normal)
    {
        glm::i32vec3 iP0 = (offsetPos + corner_index[v0] * LOD);
        glm::vec3 P0{ iP0.x, iP0.y, iP0.z };

        glm::i32vec3 iP1 = (offsetPos + corner_index[v1] * LOD);
        glm::vec3 P1{ iP1.x, iP1.y, iP1.z };

        glm::vec3 Q = interpolate_voxel_vector(t, P0, P1);

        mesh->vertices.push_back(Q);
        mesh->normals.push_back(normal);
    }

    void polygonize_cell(voxel_chunk const * const chunk, glm::i32vec3& offset, const glm::i32vec3& position, const i16 LOD, RegularCellCache* cache, voxel_mesh* mesh)
    {
        offset += position * LOD;

        u8 direction_mask = (u8)((position.x > 0 ? 1 : 0) | ((position.z > 0 ? 1 : 0) << 1) | ((position.y > 0 ? 1 : 0) << 2));

        i8 density[8];

        for (int i = 0; i < 8; i++)
        {
            glm::i32vec3 index{ offset + corner_index[i] * i32(LOD) };
            f32 dist = chunk->data[index.x + index.y * CHUNK_SIZE + index.z * CHUNK_SIZE_SQUARED].distance;
            density[i] = (dist > 0) ? 1 : ((dist < 0) ? -1 : 0);
        }

        u8 caseCode = getCaseCode(density);

        if ((caseCode ^ ((density[7] >> 7) & 0xFF)) == 0)
        {
            return;
        }

        glm::vec3 corner_normals[8];
        for (int i = 0; i < 8; i++)
        {
            auto p = offset + corner_index[i] * i32(LOD);
            float nx = (get_cell(chunk, p + glm::i32vec3{ 1, 0, 0 }) - get_cell(chunk, p - glm::i32vec3{ 1, 0, 0 })) * 0.5f;
            float ny = (get_cell(chunk, p + glm::i32vec3{ 0, 1, 0 }) - get_cell(chunk, p - glm::i32vec3{ 0, 1, 0 })) * 0.5f;
            float nz = (get_cell(chunk, p + glm::i32vec3{ 0, 0, 1 }) - get_cell(chunk, p - glm::i32vec3{ 0, 0, 1 })) * 0.5f;

            corner_normals[i] = glm::normalize(glm::vec3{ nx, ny, nz });
        }

        const u8 regularCellClass = lengyel::regularCellClass[caseCode];
        u16 vertexLocations[12];
        memcpy(&vertexLocations[0], &lengyel::regularVertexData[caseCode][0], sizeof(u16) * 12);


        lengyel::RegularCellData c = lengyel::regularCellData[regularCellClass];
        long vertexCount = c.GetVertexCount();
        long triangleCount = c.GetTriangleCount();
        std::vector<u16> mappedIndizes;
        mappedIndizes.resize(vertexCount);

        for (int i = 0; i < vertexCount; i++)
        {
            u8 edge = (u8)(vertexLocations[i] >> 8);
            u8 reuseIndex = (u8)(edge & 0xF); //Vertex id which should be created or reused 1,2 or 3
            u8 rDir = (u8)(edge >> 4); //the direction to go to reach a previous cell for reusing 

            u8 v1 = (u8)((vertexLocations[i]) & 0x0F); //Second Corner Index
            u8 v0 = (u8)((vertexLocations[i] >> 4) & 0x0F); //First Corner Index

            i8 d0 = density[v0];
            i8 d1 = density[v1];

            assert(v1 > v0 && "this is wrong !");

            i32 t = (d1 << 8) / (d1 - d0);
            i32 u = 0x0100 - t;
            f32 t0 = t / 256.0f;
            f32 t1 = u / 256.0f;

            i32 index = -1;

            if (v1 != 7 && (rDir & direction_mask) == rDir)
            {
                ReuseCell* cell = cache->GetReusedIndex(position, rDir);
                index = cell->Verts[reuseIndex];
            }

            if (index == -1)
            {
                glm::vec3 normal = corner_normals[v0] * t0 + corner_normals[v1] * t1;
                generate_vertex(offset, mesh, LOD, t, v0, v1, d0, d1, normal);
                index = mesh->vertices.size() - 1;
            }

            if ((rDir & 8) != 0)
            {
                cache->SetReusableIndex(position, reuseIndex, mesh->vertices.size()-1);
            }

            mappedIndizes[i] = (u16)index;
        }

        for (int t = 0; t < triangleCount; t++)
        {
            for (int i = 0; i < 3; i++)
            {
                mesh->indices.push_back(mappedIndizes[c.vertexIndex[t * 3 + i]]);
            }
        }
    }

    void transvoxel(voxel_chunk const * const chunk, const i16 LOD, voxel_mesh* mesh)
    {
        RegularCellCache cache(CHUNK_SIZE * 10);
        for (int x = 0; x < CHUNK_SIZE; x++)
        {
            for (int y = 0; y < CHUNK_SIZE; y++)
            {
                for (int z = 0; z < CHUNK_SIZE; z++)
                {
                    polygonize_cell(chunk, glm::i32vec3{ x, y, z }, glm::i32vec3(x, y, z), LOD, &cache, mesh);
                }
            }
        }
    }
}
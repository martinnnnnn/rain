#pragma once

#include "glm.hpp"
#include <vector>
#include "voxel_cell.h"
#include "voxel_chunk.h"

namespace rain::engine::isosurface
{
    struct ReuseCell
    {
        std::vector<i32> Verts;

        ReuseCell(int size)
        {
            Verts.resize(size, -1);
        }
    };

    struct RegularCellCache
    {
        std::vector<std::vector<ReuseCell*>> cache;
        int chunkSize;

        RegularCellCache(i32 size)
        {
            chunkSize = size;
            cache.resize(2);

            cache[0].resize(chunkSize * chunkSize);
            cache[1].resize(chunkSize * chunkSize);

            for (int i = 0; i < chunkSize * chunkSize; i++)
            {
                cache[0][i] = new ReuseCell(4);
                cache[1][i] = new ReuseCell(4);
            }
        }

        ReuseCell* GetReusedIndex(const glm::i32vec3& pos, i8 rDir)
        {
            int rx = rDir & 0x01;
            int rz = (rDir >> 1) & 0x01;
            int ry = (rDir >> 2) & 0x01;

            int dx = pos.x - rx;
            int dy = pos.y - ry;
            int dz = pos.z - rz;

            assert(dx >= 0 && dy >= 0 && dz >= 0);
            return cache[dx & 1][dy * chunkSize + dz];
        }


        void SetReusableIndex(const glm::i32vec3& pos, i8 reuseIndex, u16 p)
        {
            cache[pos.x & 1][pos.y * chunkSize + pos.z]->Verts[reuseIndex] = p;
        }
    };


    struct voxel_mesh
    {
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<u16> indices;
    };

    void transvoxel(voxel_chunk const * const chunk, const i16 LOD, voxel_mesh* mesh);
}

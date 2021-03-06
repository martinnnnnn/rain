#include  "surface_extractor.h"

#include "oct_tree_node.h"
#include "engine/voxel/transvoxel_tables.h"
#include "engine/core/profiler.h"
#include <future>
#include <array>

namespace rain::engine::voxel2
{
    namespace
    {
        constexpr ivec3 CORNER_INDEXES[]
        {
            ivec3(0, 0, 0),
            ivec3(1, 0, 0),
            ivec3(0, 0, 1),
            ivec3(1, 0, 1),
            ivec3(0, 1, 0),
            ivec3(1, 1, 0),
            ivec3(0, 1, 1),
            ivec3(1, 1, 1)
        };

        u8 get_case_code(Sample density[8])
        {
            u8 code = 0;
            u8 konj = 0x01;
            for (i32 i = 0; i < 8; i++)
            {
                code |= u8((density[i].value >> (8 - 1 - i)) & konj);
                konj <<= 1;
            }

            return code;
        }

	    void polygonize_cell(VolumeData& volume_data, const ivec3& offset_position, const ivec3& pos, const i32 LOD, VoxelMesh* mesh, i32& caseCountValueCount)
	    {
			const ivec3 absolute_position = offset_position + pos * LOD;

		    u8 direction_mask = u8((pos.x > 0 ? 1 : 0) | ((pos.z > 0 ? 1 : 0) << 1) | ((pos.y > 0 ? 1 : 0) << 2));

		    Sample density[8];


			//async version
			//	std::array<std::future<Sample>, 8> values_future;
			//for (int i = 0; i < 8; i++)
			//{
			//	values_future[i] = std::async([&]()
			//	{
			//		return volume_data.get_tf(absolute_position + CORNER_INDEXES[i] * LOD);
			//	});
			//}

			//for (int i = 0; i < 8; i++)
			//{
			//	density[i] = values_future[i].get();
			//}

            for (int i = 0; i < 8; i++)
            {
                density[i] = volume_data.get(absolute_position + CORNER_INDEXES[i] * LOD);
            }

		    u8 case_code = get_case_code(density);

		    if ((case_code ^ ((density[7].value >> 7) & 0xFF)) == 0)
		    {
			    return;
		    }

            caseCountValueCount++;

		    glm::vec3 corner_normals[8];
		    for (i32 i = 0; i < 8; i++)
		    {
			    ivec3 p = absolute_position + CORNER_INDEXES[i] * LOD;

				const glm::vec3 normal =
				{
					f32(volume_data.get(p + ivec3(1, 0, 0)).value - volume_data.get(p - ivec3(1, 0, 0)).value) * 0.5f,
					f32(volume_data.get(p + ivec3(0, 1, 0)).value - volume_data.get(p - ivec3(0, 1, 0)).value) * 0.5f,
					f32(volume_data.get(p + ivec3(0, 0, 1)).value - volume_data.get(p - ivec3(0, 0, 1)).value) * 0.5f
				};

				corner_normals[i] = glm::normalize(normal);
		    }

		    u8 regularCellClass = lengyel::regularCellClass[case_code];
		    const u16* vertexLocations = lengyel::regularVertexData[case_code];

		    lengyel::RegularCellData c = lengyel::regularCellData[regularCellClass];
		    long vertex_count = c.GetVertexCount();
		    long triangle_count = c.GetTriangleCount();
		    std::vector<u32> real_indices;
		    real_indices.resize(c.GetTriangleCount() * 3);

		    for (int i = 0; i < vertex_count; i++)
		    {
			    u8 edge = u8(vertexLocations[i] >> 8);
			    u8 reuseIndex = u8(edge & 0xF); //Vertex id which should be created or reused 1,2 or 3
			    u8 rDir = u8(edge >> 4); //the direction to go to reach a previous cell for reusing 

			    u8 v1 = u8((vertexLocations[i]) & 0x0F); //Second Corner Index
			    u8 v0 = u8((vertexLocations[i] >> 4) & 0x0F); //First Corner Index

			    const Sample& d0 = density[v0];
			    const Sample& d1 = density[v1];

                const f32 t = f32(d1.value) / f32(d1.value - d0.value);

			    u32 index = LONG_MAX;

			    //if (UseCache && v1 != 7 && (rDir & directionMask) == rDir)
			    //{
			    //	Debug.Assert(reuseIndex != 0);
			    //	ReuseCell cell = cache.GetReusedIndex(pos, rDir);
			    //	index = cell.Verts[reuseIndex];
			    //}

			    if (index == LONG_MAX)
			    {
                    const glm::vec3 P0 = (absolute_position + CORNER_INDEXES[v0] * LOD);
                    const glm::vec3 P1 = (absolute_position + CORNER_INDEXES[v1] * LOD);

                    const glm::vec3 position = P0 * t + P1 * (1.0f - t);
                    const glm::vec3 normal = corner_normals[v0] * t + corner_normals[v1] * (1.0f - t);

                    mesh->vertices.push_back(VoxelVertex{ position, normal });

					index = u32(mesh->vertices.size() - 1);
			    }

			    //if ((rDir & 8) != 0)
			    //{
			    //	cache.SetReusableIndex(pos, reuseIndex, mesh.LatestAddedVertIndex());
			    //}

			    real_indices[i] = u32(index);
		    }


			std::reverse(c.vertexIndex, c.vertexIndex + (c.GetTriangleCount() * 3));

		    for (int t = 0; t < triangle_count; t++)
		    {
			    for (int i = 0; i < 3; i++)
			    {
				    mesh->indices.push_back(real_indices[c.vertexIndex[t * 3 + i]]);
			    }
		    }
	    }
    }

    void SurfaceExtractor::transvoxel(VolumeData& volume_data, const ivec3& min, const VolumeSize& size, const OCTTREE_LOD LOD, VoxelMesh* mesh)
    {
        assert(LOD > 0 && "LOD must at least be 1");
        //RAIN_PROFILE("SurfaceExtractor::transvoxel");

        i32 caseCountValueCount = 0;
        i32 cellCount = 0;
        for (i32 x = 0; x < size.value; x++)
        {
            for (i32 y = 0; y < size.value; y++)
            {
                for (i32 z = 0; z < size.value; z++)
                {
                    ivec3 sample_position(x, y, z);
                    polygonize_cell(volume_data, min, sample_position, LOD, mesh, caseCountValueCount);
                    cellCount++;
                }
            }
        }

        //RAIN_LOG("Generated %u vertices and %u indices for LOD %d with %d valid case codes in %d cells", mesh->vertices.size(), mesh->indices.size(), i32(LOD), caseCountValueCount, cellCount);

    }
}








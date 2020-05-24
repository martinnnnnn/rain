#pragma once

#include "glm.hpp"
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include "float.h"

#include "core/core.h"
#include "transvoxel_tables.h"
#include "engine/core/context.h"

namespace rain::engine::voxel
{
	typedef glm::ivec3 ivec3;
	typedef glm::uvec3 uvec3;

	struct ivec3_hasher
	{
		std::size_t operator()(const ivec3& position) const
		{
			std::size_t seed = 3;

			seed ^= position.x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= position.y + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= position.z + 0x9e3779b9 + (seed << 6) + (seed >> 2);

			return seed;
		}

		bool operator()(const ivec3& position_a, const ivec3& position_b) const
		{
			return position_a.x == position_b.x
				&& position_a.y == position_b.y
				&& position_a.z == position_b.z;
		}

	};

	struct Sample
	{
		i8 value;
	};

	template<u32 S>
	struct VolumeChunk
	{
		static constexpr u32 SIZE = S;
		static constexpr u32 SIZE_SQUARED = S * S;
		static constexpr u32 SIZE_CUBED = S * S * S;

		inline const Sample& at(u32 offset) const;
		inline const Sample& at(u32 x, u32 y, u32 z) const;
		inline const Sample& at(uvec3 position) const;

		ivec3 position;
		Sample samples[SIZE_CUBED];
	};

	template<u32 S>
	struct VolumeChunkHasher
	{
		std::size_t operator()(VolumeChunk<S>* const& volume_chunk) const
		{
			std::size_t seed = 3;

			seed ^= volume_chunk.position.x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= volume_chunk.position.y + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= volume_chunk.position.z + 0x9e3779b9 + (seed << 6) + (seed >> 2);

			return seed;
		}

		bool operator()(VolumeChunk<S>* const& volume_chunk_a, VolumeChunk<S>* const& volume_chunk_b) const
		{
			return volume_chunk_a.position.x == volume_chunk_b.position.x
				&& volume_chunk_a.position.y == volume_chunk_b.position.y
				&& volume_chunk_a.position.z == volume_chunk_b.position.z;
		}
	};

	struct VoxelVertex
	{
		glm::vec3 vertice;
		glm::vec3 normal;
	};

	struct VoxelMesh
	{
		std::vector<VoxelVertex> vertices;
		std::vector<u16> indices;
		u32 vao;
		u32 vbo;
		u32 ebo;
	};

	struct VoxelChunk
	{
		glm::ivec3	position;
		u32			size;
		i32			LOD;
		VoxelMesh	mesh;
	};


	template<u32 VolumeChunkSize>
	struct VolumeData	
	{
		using VolumeChunkMap = std::unordered_map<ivec3, VolumeChunk<VolumeChunkSize>*, ivec3_hasher, ivec3_hasher>;

		void transvoxel(VoxelChunk* voxel_chunk);

	private:

		static constexpr ivec3 corner_indexes[]
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

		static glm::vec3					interpolate_voxel_vector(const long t, const glm::vec3& p0, const glm::vec3& p1);
		static void							generate_vertex(const ivec3& offsetPos, i32 LOD, long t, u8 v0, u8 v1, const glm::vec3& normal, VoxelMesh* mesh);
		static u8							get_case_code(Sample density[8]);

		void								polygonize_cell(const ivec3& offset_pos, const ivec3& pos, const i32 LOD, VoxelMesh* mesh);
		Sample								get_sample(const ivec3& sample_position);

		const VolumeChunk<VolumeChunkSize>& get_chunk(const ivec3& chunk_position);
		void								add_chunk(VolumeChunk<VolumeChunkSize>* chunk);
		void								replace_chunk(VolumeChunk<VolumeChunkSize>* chunk);

		ivec3								get_chunk_index(ivec3 sample_position);
		bool 								contains(ivec3 chunk_position) { return data.find(chunk_position) != data.end(); }
		
		VolumeChunkMap data;
	};

	struct VoxelChunkHasher
	{
		std::size_t operator()(VoxelChunk* const& voxel_chunk) const
		{
			std::size_t seed = 3;

			seed ^= voxel_chunk->position.x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= voxel_chunk->position.y + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= voxel_chunk->position.z + 0x9e3779b9 + (seed << 6) + (seed >> 2);

			return seed;
		}

		bool operator()(VoxelChunk* const& voxel_chunk_a, VoxelChunk* const& voxel_chunk_b) const
		{
			return voxel_chunk_a->position.x == voxel_chunk_b->position.x
				&& voxel_chunk_a->position.y == voxel_chunk_b->position.y
				&& voxel_chunk_a->position.z == voxel_chunk_b->position.z;
		}
	};

	template<u32 VolumeChunkSize>
	struct TransvoxelManager
	{
		using VoxelMap = std::unordered_map<ivec3, VoxelChunk*, ivec3_hasher, ivec3_hasher>;

		TransvoxelManager(const ivec3& position = ivec3())
			: position(position)
		{
		}

		//void load_chunk(const ivec3& position)
		//{
		//	assert(chunks.contains(position));

		//	RAIN_RENDERER->init_transvoxel2(&(chunks[position]->mesh));
		//}

		//void draw_chunk(const ivec3& position)
		//{
		//	RAIN_RENDERER->draw_transvoxel2(chunks[position]->mesh.vao, chunks[position]->mesh.indices.count, position);

		//}

	//private:
		ivec3 position;
		VolumeData<VolumeChunkSize> volume_data;
		VoxelMap chunks;
	};


	template<u32 S>
	const Sample& VolumeChunk<S>::at(u32 offset) const
	{
		return samples[offset];
	}

	template<u32 S>
	inline const Sample& VolumeChunk<S>::at(u32 x, u32 y, u32 z) const
	{
		return at(x + y * SIZE + z * SIZE_SQUARED);
	}

	template<u32 S>
	inline const Sample& VolumeChunk<S>::at(uvec3 position) const
	{
		return at(position.x, position.y, position.z);
	}

	template<u32 VolumeChunkSize>
	glm::vec3 VolumeData<VolumeChunkSize>::interpolate_voxel_vector(const long t, const glm::vec3& p0, const glm::vec3& p1)
	{
		long u = 0x0100 - t; //256 - t
		float s = 1.0f / 256.0f;
		glm::vec3 Q = p0 * t + p1 * u; //Density Interpolation
		Q *= s; // shift to shader ! 
		return Q;
	}

	template<u32 VolumeChunkSize>
	void VolumeData<VolumeChunkSize>::generate_vertex(const ivec3& offsetPos, i32 LOD, long t, u8 v0, u8 v1, const glm::vec3& normal, VoxelMesh* mesh)
	{
		glm::vec3 P0 = (offsetPos + corner_indexes[v0] * LOD);
		glm::vec3 P1 = (offsetPos + corner_indexes[v1] * LOD);

		glm::vec3 Q = interpolate_voxel_vector(t, P0, P1);
		mesh->vertices.push_back(VoxelVertex{ Q, normal });
	}


	template<u32 VolumeChunkSize>
	u8 VolumeData<VolumeChunkSize>::get_case_code(Sample density[8])
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

	template<u32 VolumeChunkSize>
	void VolumeData<VolumeChunkSize>::polygonize_cell(const ivec3& offset_pos, const ivec3& pos, const i32 LOD, VoxelMesh* mesh)
	{
		const ivec3 offset_position = offset_pos + pos * LOD;

		u8 direction_mask = u8((pos.x > 0 ? 1 : 0) | ((pos.z > 0 ? 1 : 0) << 1) | ((pos.y > 0 ? 1 : 0) << 2));

		Sample density[8];

		for (int i = 0; i < 8; i++)
		{
			density[i] = get_sample(offset_position + corner_indexes[i] * LOD);
		}

		u8 case_code = get_case_code(density);

		if ((case_code ^ ((density[7].value >> 7) & 0xFF)) == 0)
		{
			return;
		}

		glm::vec3 corner_normals[8];
		for (i32 i = 0; i < 8; i++)
		{
			ivec3 p = offset_position + corner_indexes[i] * LOD;
			corner_normals[i].x = (get_sample(p + ivec3(1, 0, 0)).value - get_sample(p - ivec3(1, 0, 0)).value) * 0.5f;
			corner_normals[i].y = (get_sample(p + ivec3(0, 1, 0)).value - get_sample(p - ivec3(0, 1, 0)).value) * 0.5f;
			corner_normals[i].z = (get_sample(p + ivec3(0, 0, 1)).value - get_sample(p - ivec3(0, 0, 1)).value) * 0.5f;

			glm::normalize(corner_normals[i]);
		}

		u8 regularCellClass = lengyel::regularCellClass[case_code];
		const u16* vertexLocations = lengyel::regularVertexData[case_code];

		lengyel::RegularCellData c = lengyel::regularCellData[regularCellClass];
		long vertex_count = c.GetVertexCount();
		long triangle_count = c.GetTriangleCount();
		std::vector<u16> real_indices;
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

			assert(v1 > v0 && "??");

			const i32 t = (d1.value << 8) / (d1.value - d0.value);
			const i32 u = 0x0100 - t;
			const f32 t0 = f32(t) / 256.0f;
			const f32 t1 = f32(u) / 256.0f;

			i16 index = -1;

			//if (UseCache && v1 != 7 && (rDir & directionMask) == rDir)
			//{
			//	Debug.Assert(reuseIndex != 0);
			//	ReuseCell cell = cache.GetReusedIndex(pos, rDir);
			//	index = cell.Verts[reuseIndex];
			//}

			if (index == -1)
			{
				glm::vec3 normal = corner_normals[v0] * t0 + corner_normals[v1] * t1;
				generate_vertex(offset_position, LOD, t, v0, v1, normal, mesh);
				index = i16(mesh->vertices.size() - 1);
			}

			//if ((rDir & 8) != 0)
			//{
			//	cache.SetReusableIndex(pos, reuseIndex, mesh.LatestAddedVertIndex());
			//}

			real_indices[i] = u16(index);
		}

		for (int t = 0; t < triangle_count; t++)
		{
			for (int i = 0; i < 3; i++)
			{
				mesh->indices.push_back(real_indices[c.vertexIndex[t * 3 + i]]);
			}
		}
	}

	template<u32 VolumeChunkSize>
	void VolumeData<VolumeChunkSize>::transvoxel(VoxelChunk* voxel_chunk)
	{
		assert(voxel_chunk->LOD > 0 && "LOD must at least be 1");
		
		for (u32 x = 0; x < voxel_chunk->size; x++)
		{
			for (u32 y = 0; y < voxel_chunk->size; y++)
			{
				for (u32 z = 0; z < voxel_chunk->size; z++)
				{
					ivec3 sample_position(x, y, z);
					polygonize_cell(voxel_chunk->position, sample_position, voxel_chunk->LOD, &(voxel_chunk->mesh));
				}
			}
		}
	}

	template<u32 VolumeChunkSize>
	Sample VolumeData<VolumeChunkSize>::get_sample(const ivec3& sample_position)
	{
		const ivec3 chunk_index = get_chunk_index(sample_position);

		if (!contains(chunk_index))
		{
			return Sample{ i8(core::simplex_noise::noise(f32(sample_position.x), f32(sample_position.y), f32(sample_position.z)) * 127.0f) };
		}

		const u32 offset_index =
			(sample_position.x - chunk_index.x * VolumeChunk<VolumeChunkSize>::SIZE) +
			(sample_position.y - chunk_index.y * VolumeChunk<VolumeChunkSize>::SIZE) * VolumeChunk<VolumeChunkSize>::SIZE +
			(sample_position.z - chunk_index.z * VolumeChunk<VolumeChunkSize>::SIZE) * VolumeChunk<VolumeChunkSize>::SIZE_SQUARED;

		return data[chunk_index]->at(offset_index);
	}

	template<u32 VolumeChunkSize>
	const VolumeChunk<VolumeChunkSize>& VolumeData<VolumeChunkSize>::get_chunk(const ivec3& chunk_position)
	{
		assert(data.contains(chunk) && "no chunk at this position");
		return *(data[chunk->position]);
	}

	template<u32 VolumeChunkSize>
	void VolumeData<VolumeChunkSize>::add_chunk(VolumeChunk<VolumeChunkSize>* chunk)
	{
		assert(!data.contains(chunk) && "Trying to add an already existing chunk");
		data.insert(chunk);
	}

	template<u32 VolumeChunkSize>
	void VolumeData<VolumeChunkSize>::replace_chunk(VolumeChunk<VolumeChunkSize>* chunk)
	{
		assert(data.contains(chunk) && "Trying to replace a missing chunk");
		delete data[chunk->position];
		data[chunk->position] = nullptr;
		data.insert(chunk);
	}

	template<u32 VolumeChunkSize>
	ivec3 VolumeData<VolumeChunkSize>::get_chunk_index(ivec3 sample_position)
	{
		i32 chunk_x = sample_position.x / VolumeChunkSize;
		if (sample_position.x < 0)
		{
			chunk_x--;
		}

		int chunk_y = sample_position.y / VolumeChunkSize;
		if (sample_position.y < 0)
		{
			chunk_y--;
		}

		int chunk_z = sample_position.z / VolumeChunkSize;
		if (sample_position.z < 0)
		{
			chunk_z--;
		}

		return ivec3{ chunk_x, chunk_y, chunk_z };
	}
}



#pragma once

#include "glm.hpp"
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "float.h"

#include "core/core.h"
#include "engine/core/context.h"
#include "assert.h"
#include "volume_chunk.h"

// implement chunk -> get pos
// chunk streaming
// async chunk streaming
// debug show of chunk streaming
// volume data reposition, scale and rotation
// make sure up is up
// sample value  generation
// render sample value as color in debug
// debug take map position into account


namespace rain::engine::voxel2
{
	struct VolumeData
	{
		VolumeData(u32 chunk_s = VolumeChunk::DEFAULT_SIZE)
			: chunk_size(chunk_s)
		{
		}

		void			set(i32 x, i32 y, i32 z, Sample sample);
		const Sample&	get(i32 x, i32 y, i32 z);

		void init_debug();
		void load_debug();
		void draw_debug();

	private:

		using VolumeChunkMap = std::unordered_map<ivec3, VolumeChunk*, ivec3_hasher, ivec3_hasher>;

		VolumeChunk*	create_chunk(const ivec3& chunk_index);
		VolumeChunk*	create_and_fill_chunk(const ivec3& chunk_index);
		void			add_chunk(VolumeChunk* chunk);
		ivec3			get_chunk_index(i32 x, i32 y, i32 z);
		bool			contains(const ivec3& chunk_index);

		glm::vec3			position;
		VolumeChunk::Size	chunk_size;
		VolumeChunkMap		data;
	};
}



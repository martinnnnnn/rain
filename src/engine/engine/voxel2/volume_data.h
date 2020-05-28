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
#include "volume_size.h"

// chunk streaming
// async chunk streaming
// debug show of chunk streaming
// render sample value as color in debug
// fix inputs
// move Size out of volume chunk

namespace rain::engine::voxel2
{
    struct VolumeData
    {
        VolumeData(u32 chunk_s = VolumeChunk::DEFAULT_SIZE)
            : chunk_size(chunk_s)
        {
        }
    
        void            set(i32 x, i32 y, i32 z, Sample sample);
    
		const Sample&   get(i32 x, i32 y, i32 z);
		//const Sample&   get(i32 x, i32 y, i32 z) const;
		const Sample&   get(const ivec3& index) { return get(index.x, index.y, index.z); }
		//const Sample&   get(const ivec3& index) const { return get(index.x, index.y, index.z); }

        void            init();

    private:

        using VolumeChunkMap = std::unordered_map<ivec3, VolumeChunk*, ivec3_hasher, ivec3_hasher>;

        VolumeChunk*    create_chunk(const ivec3& chunk_index);
        VolumeChunk*    create_add_and_fill_chunk(const ivec3& chunk_index);
        void            add_chunk(VolumeChunk* chunk);
        ivec3           get_chunk_index(i32 x, i32 y, i32 z) const;
        bool            contains(const ivec3& chunk_index) const;

        core::transform     transform;
        VolumeSize          chunk_size;
        VolumeChunkMap      data;
    };
}



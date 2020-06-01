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


namespace rain::engine::voxel2
{
    struct VolumeChunkHandler
    {
        struct AsyncInfo
        {
            std::mutex m;
            std::condition_variable cv;
        };

        using VolumeChunkMap = std::unordered_map<ivec3, VolumeChunk*, ivec3_hasher, ivec3_hasher>;

        VolumeChunkHandler(i32 chunk_s = VolumeChunk::DEFAULT_SIZE)
            : chunk_size(chunk_s)
        {}

        void get(const ivec3& index, VolumeChunk*& chunk, AsyncInfo& info)
        {
            std::unique_lock<std::mutex> lk(info.m);
            std::unique_lock<std::mutex> lock(mutex);

            if (!contains(index))
            {
                RAIN_LOG("Adding new chunk (%d, %d, %d)", index.x, index.y, index.z);
                create_add_and_fill_chunk(index);
            }

            chunk = data[index];

            lk.unlock();
            info.cv.notify_one();
            lock.unlock();
            condition.notify_one();
        }

    private:
        VolumeChunk* create_chunk(const ivec3& chunk_index)
        {
            return new VolumeChunk(chunk_index, chunk_size.value);
        }

        void add_chunk(VolumeChunk* chunk)
        {
            data[chunk->index] = chunk;
        }

        VolumeChunk* create_add_and_fill_chunk(const ivec3& chunk_index)
        {
            VolumeChunk* new_chunk = create_chunk(chunk_index);
            add_chunk(new_chunk);
            new_chunk->init_values_simplex();

            return new_chunk;
        }

        inline bool contains(const ivec3& chunk_index) const
        {
            return data.find(chunk_index) != data.end();
        }

        VolumeChunkMap data;
        VolumeSize chunk_size;

        mutable std::mutex mutex;
        std::condition_variable condition;
    };

    struct VolumeData
    {
        VolumeData(i32 chunk_s = VolumeChunk::DEFAULT_SIZE);
    
        //void            set(i32 x, i32 y, i32 z, const Sample& sample);
    
		const Sample&   get(i32 x, i32 y, i32 z);
		//const Sample&   get(i32 x, i32 y, i32 z) const;
		const Sample&   get(const ivec3& index) { return get(index.x, index.y, index.z); }
		//const Sample&   get(const ivec3& index) const { return get(index.x, index.y, index.z); }

		// Thread safe get
		//const Sample&	get_tf(i32 x, i32 y, i32 z);
		//const Sample&	get_tf(const ivec3& index) { return get_tf(index.x, index.y, index.z); }
		//void			update();

    private:

		//using VolumeChunkMap = std::unordered_map<ivec3, VolumeChunk*, ivec3_hasher, ivec3_hasher>;

  //      VolumeChunk*    create_chunk(const ivec3& chunk_index);
  //      VolumeChunk*    create_add_and_fill_chunk(const ivec3& chunk_index);
		//void            add_chunk(VolumeChunk* chunk);
        ivec3           get_chunk_index(i32 x, i32 y, i32 z) const;
		//inline bool     contains(const ivec3& chunk_index) const;
		//inline bool     contains(VolumeChunk* chunk) const;

        core::transform     transform;
        VolumeSize          chunk_size;
		//VolumeChunkMap      data;
        VolumeChunkHandler data_handler;

		//core::queue_tf<VolumeChunk*> chunks_to_add;
	};


	//bool VolumeData::contains(const ivec3& chunk_index) const
	//{
	//	return data.find(chunk_index) != data.end();
	//}

	//bool VolumeData::contains(VolumeChunk* chunk) const
	//{
	//	return contains(chunk->index);
	//}
}



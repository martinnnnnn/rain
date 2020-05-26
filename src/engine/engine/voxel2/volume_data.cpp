#include  "volume_data.h"

#include "engine/gfx/ogl/ogl_renderer.h"
#include "engine/core/context.h"
#include "engine/data/data_system.h"
#include "gtx/transform.hpp"
#include "engine/game/world.h"

namespace rain::engine::voxel2
{
	void VolumeData::set(i32 x, i32 y, i32 z, Sample sample)
	{
		ivec3 chunk_index = get_chunk_index(x, y, z);

		VolumeChunk* chunk = nullptr;

		if (!contains(chunk_index))
		{
			create_add_and_fill_chunk(chunk_index);
			//chunk = create_chunk(chunk_index);
			//add_chunk(chunk);
		}
		else
		{
			chunk = data[chunk_index];
		}

		chunk->set(sample, chunk->offset_from_absolute(x, y, z));
	}

	const Sample& VolumeData::get(i32 x, i32 y, i32 z)
	{
		ivec3 chunk_index = get_chunk_index(x, y, z);

		//assert(contains(chunk_index) && "Trying to access an element not present in data");
		if (!contains(chunk_index))
		{
			create_add_and_fill_chunk(chunk_index);
		}

		return data[chunk_index]->get_absolute(x, y, z);
	}


	void VolumeData::init()
	{
		transform.position = glm::vec3{ 5, 0, 5 };
		transform.scale = glm::vec3{ 2, 1, 1 };
		transform.orientation = glm::quat();
	}

	VolumeChunk* VolumeData::create_chunk(const ivec3& chunk_index)
	{
		return new VolumeChunk(chunk_index, chunk_size.value);
	}

	VolumeChunk* VolumeData::create_add_and_fill_chunk(const ivec3& chunk_index)
	{
		//RAIN_LOG("creating chunk : (%d, %d, %d)", chunk_index.x, chunk_index.y, chunk_index.z);
		VolumeChunk* new_chunk = create_chunk(chunk_index);
        add_chunk(new_chunk);

		for (i32 i = 0; i < i32(chunk_size.value); ++i)
		{
			for (i32 j = 0; j < i32(chunk_size.value); ++j)
			{
				for (i32 k = 0; k < i32(chunk_size.value); ++k)
				{
					const ivec3 sample_map_pos = (chunk_index * i32(chunk_size.value)) + ivec3{ i, j, k };
					const glm::vec3 absolute_pos = transform.position + glm::vec3(sample_map_pos);
                    const f32 distf = core::simplex_noise::noise(
                        f32(sample_map_pos.x) / f32(chunk_size.value),
                        f32(sample_map_pos.y) / f32(chunk_size.value), 
                        f32(sample_map_pos.z) / f32(chunk_size.value)) * 127.0f;

                    set(sample_map_pos.x, sample_map_pos.y, sample_map_pos.z, Sample(i8(distf)));
				}
			}
		}

		return new_chunk;
	}

	void VolumeData::add_chunk(VolumeChunk* chunk)
	{
		if (!contains(chunk->index))
		{
			data[chunk->index] = chunk;
		}
	} 

	ivec3 VolumeData::get_chunk_index(i32 x, i32 y, i32 z)
	{
		i32 xI = x / i32(chunk_size.value);
		if (x < 0)
		{
			xI--;
		}

		i32 yI = y / i32(chunk_size.value);
		if (y < 0)
		{
			yI--;
		}

		i32 zI = z / i32(chunk_size.value);
		if (z < 0)
		{
			zI--;
		}

		return ivec3(xI, yI, zI);
	}

	bool VolumeData::contains(const ivec3& chunk_index)
	{
		return data.find(chunk_index) != data.end();
	}
}








#include  "volume_data.h"

#include "engine/gfx/ogl/ogl_renderer.h"
#include "engine/core/context.h"
#include "engine/data/data_system.h"
#include "gtx/transform.hpp"
#include "engine/game/world.h"
#include "oct_tree.h"
#include "engine/core/profiler.h"

namespace rain::engine::voxel2
{
    VolumeData::VolumeData(i32 chunk_s)
        : chunk_size(chunk_s)
    {
        transform.position = glm::vec3{ 5, 0, 5 };
        transform.scale = glm::vec3{ 2, 1, 1 };
        transform.orientation = glm::quat();
    }

	void VolumeData::set(i32 x, i32 y, i32 z, const Sample& sample)
	{
		ivec3 chunk_index = get_chunk_index(x, y, z);
		VolumeChunk* chunk = data[chunk_index];
		chunk->set_global(sample, x, y, z);
	}


	const Sample& VolumeData::get(i32 x, i32 y, i32 z)
	{
		ivec3 chunk_index = get_chunk_index(x, y, z);

		if (!contains(chunk_index))
		{
			create_add_and_fill_chunk(chunk_index);
		}

		return data[chunk_index]->get_global(x, y, z);
	}

    const Sample& VolumeData::get(i32 x, i32 y, i32 z) const
    {
        const ivec3 chunk_index = get_chunk_index(x, y, z);

        assert(!contains(chunk_index) && "Trying to access an element not present in data");

        return data.at(chunk_index)->get_global(x, y, z);
    }

	VolumeChunk* VolumeData::create_chunk(const ivec3& chunk_index)
	{
		return new VolumeChunk(chunk_index, chunk_size.value);
	}

	VolumeChunk* VolumeData::create_add_and_fill_chunk(const ivec3& chunk_index)
	{
        RAIN_PROFILE("VolumeData::create_add_and_fill_chunk");

		VolumeChunk* new_chunk = create_chunk(chunk_index);
        add_chunk(new_chunk);
        new_chunk->init_values_simplex();

		return new_chunk;
	}

	void VolumeData::add_chunk(VolumeChunk* chunk)
	{
		if (!contains(chunk->index))
		{
			data[chunk->index] = chunk;
		}
	} 

	ivec3 VolumeData::get_chunk_index(i32 x, i32 y, i32 z) const
	{
		i32 xI = x / chunk_size.value;
		if (x < 0 && xI == 0)
		{
			xI--;
		}

		i32 yI = y / chunk_size.value;
		if (y < 0 && yI == 0)
		{
			yI--;
		}

		i32 zI = z / chunk_size.value;
		if (z < 0 && zI == 0)
		{
			zI--;
		}

		return ivec3(xI, yI, zI);
	}

	bool VolumeData::contains(const ivec3& chunk_index) const
	{
		return data.find(chunk_index) != data.end();
	}
}








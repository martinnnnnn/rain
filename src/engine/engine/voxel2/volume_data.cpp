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


		//int offset_index =
		//	(x - chunk_index.x * chunk_size.value) +
		//	(y - chunk_index.y * chunk_size.value) * chunk_size.value +
		//	(z - chunk_index.z * chunk_size.value) * chunk_size.squared;

		return data[chunk_index]->get_absolute(x, y, z);
	}


	void VolumeData::init_debug()
	{
		transform.position = glm::vec3{ 5, 0, 5 };
		transform.scale = glm::vec3{ 2, 1, 1 };
		transform.orientation = glm::quat();

		RAIN_LOG("init debug");
		
		//for (i32 i = 0; i < i32(chunk_size.value) * 5; ++i)
		//{
		//	for (i32 j = -i32(chunk_size.value); j < i32(chunk_size.value); ++j)
		//	{
		//		for (i32 k = 0; k < i32(chunk_size.value); ++k)
		//		{
		//			set(i, j, k, Sample(i8(core::simplex_noise::noise(f32(i), f32(j), f32(k)) * 127)));
		//		}
		//	}
		//}
	}

	u32 vao, vbo, vbo2;
	handle<Texture> const * texture_handle;
	std::vector<glm::mat4> model_matrices;

	void VolumeData::load_debug()
	{
		//RAIN_LOG("load debug");
		//model_matrices.reserve(10'000'000);

		//for (auto item : data)
		//{
		//	const glm::vec3 chunk_pos = item.second->index;
		//	RAIN_LOG("loading chunk at (%f, %f, %f)", chunk_pos.x, chunk_pos.y, chunk_pos.z);
		//	for (u32 u = 0; u < chunk_size.cubed; ++u)
		//	{
		//		glm::vec3 sample_pos = chunk_size.get_position(u);
		//		if (item.second->get(u).value > 0)
		//		{
		//			model_matrices.emplace_back(glm::translate(glm::mat4(1), chunk_pos * f32(chunk_size.value) + sample_pos) /** glm::scale(glm::mat4(1), glm::vec3(0.1f, 0.1f, 0.1f))*/);
		//		}
		//	}
		//}

		//RAIN_LOG("%d cubes to drawn", model_matrices.size());
		//RAIN_RENDERER->init_instancing_cube(model_matrices, vao, vbo, vbo2);

		//texture_handle = RAIN_FIND_DATA_FROM_PATH(Texture, RAIN_CONFIG->data_root + "/awesomeface.png");
	}

	void VolumeData::reload_debug()
	{
		//set(i32(chunk_size.value) * 6, i32(chunk_size.value) * 6, i32(chunk_size.value) * 6, Sample(5));

		//model_matrices.clear();
		//for (auto item : data)
		//{
		//	const glm::vec3 chunk_pos = item.second->index;
		//	for (u32 u = 0; u < chunk_size.cubed; ++u)
		//	{
		//		glm::vec3 sample_pos = chunk_size.get_position(u);
		//		if (item.second->get(u).value > 0)
		//		{
		//			model_matrices.emplace_back(glm::translate(glm::mat4(1), chunk_pos * f32(chunk_size.value) + sample_pos) /** glm::scale(glm::mat4(1), glm::vec3(0.1f, 0.1f, 0.1f))*/);
		//		}
		//	}
		//}

		//RAIN_LOG("%d cubes to drawn", model_matrices.size());
		//RAIN_RENDERER->reload_instancing_cube(vao, vbo, model_matrices);
	}

	void VolumeData::draw_debug()
	{
		//RAIN_RENDERER->draw_instancing_cube(vao, model_matrices.size(), get_transform_matrix(transform), texture_handle->data, RAIN_WORLD->main_camera.transform->position);
	}

	VolumeChunk* VolumeData::create_chunk(const ivec3& chunk_index)
	{
		return new VolumeChunk(chunk_index, chunk_size.value);
	}

	VolumeChunk* VolumeData::create_add_and_fill_chunk(const ivec3& chunk_index)
	{
		RAIN_LOG("creating chunk : (%d, %d, %d)", chunk_index.x, chunk_index.y, chunk_index.z);
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








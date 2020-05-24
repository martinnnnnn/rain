#include  "oct_tree.h"


#include "engine/gfx/ogl/ogl_renderer.h"
#include "engine/core/context.h"
#include "engine/game/world.h"

namespace rain::engine::voxel2
{
	OctTreeNode::OctTreeNode(VolumeData& vol_data, const ivec3& m, OCTTREE_LOD lod, i32 s, f32 max_dist)
		: volume_data(vol_data)
		, min(m)
		, LOD(lod)
		, size(s)
		, max_distance(max_dist)
		, has_children(false)
		, is_loaded(false)
		, vao(0)
		, vbo_instances(0)
		, vbo_cube(0)
		, texture_handle(nullptr)
	{
		children.fill(nullptr);
		mats.reserve(size * size * size);
	}

	ivec3 OctTreeNode::get_center() const
	{
		return ivec3{ min.x + (size * LOD / 2), min.y + (size * LOD / 2) , min.z + (size * LOD / 2) };
	}

	bool OctTreeNode::point_collision(const glm::vec3& other) const
	{
		return (other.x >= min.x && other.x <= min.x + size * LOD)
			&& (other.y >= min.y && other.y <= min.y + size * LOD)
			&& (other.z >= min.z && other.z <= min.z + size * LOD);
	}

	void OctTreeNode::create_children()
	{
		const ivec3 center = get_center();
		const OCTTREE_LOD child_lod = LOD / 2;

		children[0] = new OctTreeNode(volume_data, ivec3{ min.x,    min.y,    min.z    }, child_lod, size, max_distance / 2.0f);
		children[1] = new OctTreeNode(volume_data, ivec3{ center.x, min.y,    min.z    }, child_lod, size, max_distance / 2.0f);
		children[2] = new OctTreeNode(volume_data, ivec3{ min.x,    min.y,    center.z }, child_lod, size, max_distance / 2.0f);
		children[3] = new OctTreeNode(volume_data, ivec3{ center.x, min.y,    center.z }, child_lod, size, max_distance / 2.0f);

		children[4] = new OctTreeNode(volume_data, ivec3{ min.x,    center.y, min.z    }, child_lod, size, max_distance / 2.0f);
		children[5] = new OctTreeNode(volume_data, ivec3{ center.x, center.y, min.z    }, child_lod, size, max_distance / 2.0f);
		children[6] = new OctTreeNode(volume_data, ivec3{ min.x,    center.y, center.z }, child_lod, size, max_distance / 2.0f);
		children[7] = new OctTreeNode(volume_data, ivec3{ center.x, center.y, center.z }, child_lod, size, max_distance / 2.0f);
	}

	void OctTreeNode::delete_children()
	{
		for (u32 u(0); u < children.size(); ++u)
		{
			delete children[u];
			children[u] = nullptr;
		}
	}

	void OctTreeNode::update_children(const glm::vec3& other)
	{
		for (u32 u(0); u < children.size(); ++u)
		{
			children[u]->update(other);
		}
	}

	void OctTreeNode::load()
	{
		RAIN_LOG("loading octtree node (%d, %d, %d)", min.x, min.y, min.z);
		for (i32 i = 0; i < size; ++i)
		{
			for (i32 j = 0; j < size; ++j)
			{
				for (i32 k = 0; k < size; ++k)
				{
					const ivec3 sample_pos = ivec3{ min.x + i * LOD, min.y + j * LOD, min.z + k * LOD };
					const Sample& sample = volume_data.get(sample_pos);

					if (sample.value > 0)
					{
						mats.emplace_back(glm::translate(glm::mat4(1), glm::vec3(sample_pos)));
					}
				}
			}
		}

		RAIN_RENDERER->init_instancing_cube(mats, vao, vbo_instances, vbo_cube);
		texture_handle = RAIN_FIND_DATA_FROM_PATH(Texture, RAIN_CONFIG->data_root + "/awesomeface.png");
	}

	void OctTreeNode::unload()
	{
		RAIN_RENDERER->delete_instancing_cube(vao, vbo_instances, vbo_cube);
	}

	void OctTreeNode::draw()
	{
		RAIN_LOG("drawing (%d, %d, %d)", min.x, min.y, min.z);
		if (has_children)
		{
			for (u32 u(0); u < children.size(); ++u)
			{
				children[u]->draw();
			}
		}
		else
		{
			RAIN_RENDERER->draw_instancing_cube(vao, mats.size(), glm::mat4(1), texture_handle->data, RAIN_WORLD->main_camera.transform->position);

			const glm::vec3 center = get_center();
			glm::vec3 color{};

			color = glm::vec3(1.0f - f32(LOD) / f32(OCTTREE_LOD_MAX), 0, 0);

			RAIN_RENDERER->draw_debug_cube(center, f32(size) * LOD, f32(size) * LOD, color);
		}
	}

	void OctTreeNode::update(const glm::vec3& other)
	{
		RAIN_LOG("udpate (%d, %d, %d)", min.x, min.y, min.z);
		const f32 distance = glm::distance(glm::vec3(get_center()), other);

		if (!point_collision(other) /*distance >= max_distance*/ && has_children)
		{
			delete_children();
			has_children = false;

			//unload();
			//is_loaded = false;
		}
		else if (point_collision(other) /*distance < max_distance*/ && !has_children && LOD > OCTTREE_LOD_MIN)
		{
			create_children();
			unload();
			is_loaded = false;
			has_children = true;
		}

		if (has_children)
		{
			update_children(other);
		}
		else
		{
			if (!is_loaded)
			{
				load();
				is_loaded = true;
			}
		}
	}

	OctTree::OctTree()
		: volume_data(nullptr)
		, size(size)
		, MAX_LOD(OCTTREE_LOD_MAX)
	{
	}

	void OctTree::init(VolumeData* volume_data, u32 size, OCTTREE_LOD max_lod)
	{
		this->volume_data = volume_data;
		this->size = size;
		this->MAX_LOD = max_lod;
		roots.emplace_back(new OctTreeNode(*volume_data, ivec3{ 0, 0, 0 }, MAX_LOD, size, 2));
	}

	void OctTree::update(const glm::vec3& other)
	{
		for (u32 u(0); u < roots.size(); ++u)
		{
			roots[u]->update(other);
		}
	}

	void OctTree::draw()
	{
		for (u32 u(0); u < roots.size(); ++u)
		{
			roots[u]->draw();
		}
	}
}








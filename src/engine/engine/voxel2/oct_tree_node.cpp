#include  "oct_tree_node.h"


#include "engine/gfx/ogl/ogl_renderer.h"
#include "engine/core/context.h"
#include "engine/core/profiler.h"
#include "engine/game/world.h"
#include "surface_extractor.h"

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

        for (const ivec3& child_min : CHILDREN_MINS)
        {
            // use this to generate children  [ min + size * LOD / 2 * child_min => chooses between min or center for child min x y z]
            //RAIN_LOG("1 :: %d, %d, %d", min + ivec3{ size * LOD / 2, size * LOD / 2, size * LOD / 2 } *child_min);
        }

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

	void OctTreeNode::load_instanting()
	{
		//RAIN_LOG("loading octtree node (%d, %d, %d) with LOD : %d", min.x, min.y, min.z, (i32)LOD);
		for (i32 i = 0; i < size; ++i)
		{
			for (i32 j = 0; j < size; ++j)
			{
				for (i32 k = 0; k < size; ++k)
				{
					const ivec3 sample_pos = ivec3{ min.x + i * LOD, min.y + j * LOD, min.z + k * LOD };
					const Sample& sample = volume_data.get(sample_pos);

					//RAIN_LOG("%u", sample.value);

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

	void OctTreeNode::unload_instanciating()
	{
		RAIN_RENDERER->delete_instancing_cube(vao, vbo_instances, vbo_cube);
	}

	void OctTreeNode::load_transvoxel()
	{
		SurfaceExtractor::transvoxel(volume_data, min, size, LOD, &voxel_mesh);
		RAIN_RENDERER->init_transvoxel_v2(&voxel_mesh);
	}

	void OctTreeNode::unload_transvoxel()
	{
        RAIN_RENDERER->delete_instancing_cube(vao, vbo_instances, vbo_cube);
		RAIN_RENDERER->delete_transvoxel_v2(voxel_mesh.vao, voxel_mesh.vbo, voxel_mesh.ebo);
		voxel_mesh.vao = 0;
		voxel_mesh.vbo = 0;
		voxel_mesh.ebo = 0;
		voxel_mesh.vertices.clear();
		voxel_mesh.indices.clear();
	}

	void OctTreeNode::draw()
	{
		if (has_children)
		{
			for (u32 u(0); u < children.size(); ++u)
			{
				children[u]->draw();
			}
		}
		else
		{
			//RAIN_RENDERER->draw_instancing_cube(vao, mats.size(), glm::mat4(1), texture_handle->data, RAIN_WORLD->main_camera.transform->position);
			RAIN_RENDERER->draw_transvoxel_v2(voxel_mesh.vao, voxel_mesh.indices.size(), glm::vec3{});


		}
	}

    void OctTreeNode::draw_debug()
    {
        if (has_children)
        {
            for (u32 u(0); u < children.size(); ++u)
            {
                children[u]->draw_debug();
            }

        }
        else
        {
            const glm::vec3 center = get_center();
            glm::vec3 color{};

            color = glm::vec3(1.0f - f32(LOD) / f32(OCTTREE_LOD_MAX), 0, 0);
            RAIN_RENDERER->draw_debug_cube(center, f32(size) * LOD, f32(size) * LOD, color);

			//for (u32 j = 0; j < voxel_mesh.vertices.size(); ++j)
			//{
			//	RAIN_RENDERER->draw_debug_line(voxel_mesh.vertices[j].vertice, voxel_mesh.vertices[j].vertice + glm::normalize(voxel_mesh.vertices[j].normal), glm::vec3(0, 1, 0));
			//}
        }
    }

	void OctTreeNode::update(const glm::vec3& other)
	{
		const f32 distance = glm::distance(glm::vec3(get_center()), other);

		if (!point_collision(other) /*distance >= max_distance*/ && has_children)
		{
			delete_children();
			has_children = false;
		}
		else if (point_collision(other) /*distance < max_distance*/ && !has_children && LOD > OCTTREE_LOD_MIN)
		{
			create_children();
			//unload_instanciating();
			unload_transvoxel();
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
				//load_instanting();
				load_transvoxel();
				is_loaded = true;
			}
		}
	}
}








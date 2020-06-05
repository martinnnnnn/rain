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
		, state(State::Initial)
		//, vao(0)
		//, vbo_instances(0)
		//, vbo_cube(0)
		//, texture_handle(nullptr)
	{
		children.fill(nullptr);
		//mats.reserve(size * size * size);
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

		//for (u32 u = 0; u < CHILDREN_MINS.size(); ++u)
		//{
		//	children[u] = new OctTreeNode(volume_data, min + ivec3{ size * LOD / 2, size * LOD / 2, size * LOD / 2 } * CHILDREN_MINS[u], child_lod, size, max_distance / 2.0f);
		//}

		children[0] = new OctTreeNode(volume_data, ivec3{ min.x,    min.y,    min.z    }, child_lod, size, max_distance / 2.0f);
		children[1] = new OctTreeNode(volume_data, ivec3{ center.x, min.y,    min.z    }, child_lod, size, max_distance / 2.0f);
		children[2] = new OctTreeNode(volume_data, ivec3{ min.x,    min.y,    center.z }, child_lod, size, max_distance / 2.0f);
		children[3] = new OctTreeNode(volume_data, ivec3{ center.x, min.y,    center.z }, child_lod, size, max_distance / 2.0f);

		children[4] = new OctTreeNode(volume_data, ivec3{ min.x,    center.y, min.z	   }, child_lod, size, max_distance / 2.0f);
		children[5] = new OctTreeNode(volume_data, ivec3{ center.x, center.y, min.z	   }, child_lod, size, max_distance / 2.0f);
		children[6] = new OctTreeNode(volume_data, ivec3{ min.x,    center.y, center.z }, child_lod, size, max_distance / 2.0f);
		children[7] = new OctTreeNode(volume_data, ivec3{ center.x, center.y, center.z }, child_lod, size, max_distance / 2.0f);
	}

	void OctTreeNode::delete_children()
	{
		for (u32 u(0); u < children.size(); ++u)
		{
			children[u]->unload_transvoxel();
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

	void OctTreeNode::load_transvoxel()
	{
		{
			RAIN_PROFILE("transvoxel");
			SurfaceExtractor::transvoxel(volume_data, min, size, LOD, &voxel_mesh);
		}
		{
			RAIN_PROFILE("load vertices");
			RAIN_RENDERER->init_transvoxel_v2(&voxel_mesh);
		}
	}

	void OctTreeNode::unload_transvoxel()
	{
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
				load_transvoxel();
				is_loaded = true;
			}
		}
	}

	void OctTreeNode::update_async(const glm::vec3& other)
	{
		switch (state)
		{
		case State::Initial:
		{
			if (point_collision(other) && LOD > OCTTREE_LOD_MIN)
			{
				state = State::ChildrenLoadInit;
			}
			else
			{
				state = State::SurfaceLoadInit;
			}
			break;
		}
        case State::ChildrenLoadInit:
        {
            create_children();
            state = State::ChildrenLoading;
            break;
        }
        case State::ChildrenLoading:
        {
            bool children_all_loaded = true;
            for (u32 u(0); u < children.size(); ++u)
            {
                if (!(children[u]->state == State::SurfaceLoaded || children[u]->state == State::ChildrenLoaded))
                {
                    children_all_loaded = false;
                }
            }

            if (children_all_loaded)
            {
                state = State::ChildrenLoaded;
            }
            break;
        }
        case State::ChildrenLoaded:
        {
            if (voxel_mesh.vao != 0)
            {
                unload_transvoxel();
            }
            if (!point_collision(other))
            {
                state = State::ChildrenUnloading;
            }
            break;
        }
		case State::ChildrenUnloading:
		{
            //delete_children();
			state = State::Initial;
			break;
		}
        // surface
		case State::SurfaceLoadInit:
		{
			async_result = std::async(std::launch::async, [&]()
			{
                //RAIN_LOG("started transvoxel");
				SurfaceExtractor::transvoxel(volume_data, min, size, LOD, &voxel_mesh);
                //RAIN_LOG("finished transvoxel");
            });
			state = State::SurfaceLoading;
			break;
		}
		case State::SurfaceLoading:
		{
			if (async_result._Is_ready())
			{
                //RAIN_LOG("async returned");
                RAIN_RENDERER->init_transvoxel_v2(&voxel_mesh);
				state = State::SurfaceLoaded;
                //RAIN_LOG("to surface loaded");
			}
			break;
		}
		case State::SurfaceLoaded:
		{
            if (children[0])
            {
                delete_children();
            }
			if (point_collision(other) && LOD > OCTTREE_LOD_MIN)
			{
                //RAIN_LOG("point_collision with LOD %d", LOD);
                state = State::SurfaceUnloading;
			}
			break;
		}
        case State::SurfaceUnloading:
        {
            //RAIN_LOG("unloading surface");
            //unload_transvoxel();
            state = State::Initial;
            break;
        }
		}

        for (u32 u(0); u < children.size(); ++u)
        {
            if (children[u])
            {
                children[u]->update_async(other);
            }
        }
	}

	void OctTreeNode::draw_async()
	{
        if (voxel_mesh.vao != 0)
        {
            RAIN_RENDERER->draw_transvoxel_v2(voxel_mesh.vao, voxel_mesh.indices.size(), glm::vec3{});
        }

        for (OctTreeNode*& child : children)
        {
            if (child)
            {
                child->draw_async();
            }
        }

		//switch (state)
		//{
		//case State::SurfaceLoaded:
		//{
  //          //RAIN_RENDERER->draw_transvoxel_v2(voxel_mesh.vao, voxel_mesh.indices.size(), glm::vec3{});
		//	break;
		//}
		//case State::ChildrenLoaded:
		//{
  //          //RAIN_LOG("drawing children");
  //          for (OctTreeNode*& child : children)
  //          {
  //              //if (child && child->state == State::SurfaceLoaded)
  //              {
  //                  child->draw_async();
  //              }
  //          }
		//	break;
		//}
		//default:
		//{
		//	break;
		//}
		//}
	}

	// obselete
	//void OctTreeNode::load_instanting()
	//{
	//	//RAIN_LOG("loading octtree node (%d, %d, %d) with LOD : %d", min.x, min.y, min.z, (i32)LOD);
	//	for (i32 i = 0; i < size; ++i)
	//	{
	//		for (i32 j = 0; j < size; ++j)
	//		{
	//			for (i32 k = 0; k < size; ++k)
	//			{
	//				const ivec3 sample_pos = ivec3{ min.x + i * LOD, min.y + j * LOD, min.z + k * LOD };
	//				const Sample& sample = volume_data.get(sample_pos);

	//				//RAIN_LOG("%u", sample.value);

	//				if (sample.value > 0)
	//				{
	//					mats.emplace_back(glm::translate(glm::mat4(1), glm::vec3(sample_pos)));
	//				}
	//			}
	//		}
	//	}

	//	RAIN_RENDERER->init_instancing_cube(mats, vao, vbo_instances, vbo_cube);
	//	texture_handle = RAIN_FIND_DATA_FROM_PATH(Texture, RAIN_CONFIG->data_root + "/awesomeface.png");
	//}

	//void OctTreeNode::unload_instanciating()
	//{
	//	RAIN_RENDERER->delete_instancing_cube(vao, vbo_instances, vbo_cube);
	//	mats.clear();
	//	vao = 0;
	//	vbo_instances = 0;
	//	vbo_cube = 0;
	//}

}








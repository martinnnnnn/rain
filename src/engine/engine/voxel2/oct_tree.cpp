#include  "oct_tree.h"


#include "engine/gfx/ogl/ogl_renderer.h"
#include "engine/core/context.h"
#include "engine/core/profiler.h"
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
        RAIN_PROFILE("loading octtree node");
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
		RAIN_RENDERER->delete_transvoxel_v2(voxel_mesh.vao, voxel_mesh.vbo, voxel_mesh.ebo);
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
			RAIN_LOG("drawing %u (%u) at (%d, %d, %d)", voxel_mesh.vao, voxel_mesh.indices.size(), min.x, min.y, min.z);
			RAIN_RENDERER->draw_transvoxel_v2(voxel_mesh.vao, voxel_mesh.indices.size(), min);
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
		roots.emplace_back(new OctTreeNode(*volume_data, ivec3{ -12, -12, -12 }, MAX_LOD, size, 2));
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

    void OctTree::draw_debug()
    {
        for (u32 u(0); u < roots.size(); ++u)
        {
            roots[u]->draw_debug();
        }
    }

	glm::vec3 SurfaceExtractor::interpolate_voxel_vector(const long t, const glm::vec3& p0, const glm::vec3& p1)
	{
		long u = 0x0100 - t; //256 - t
		float s = 1.0f / 256.0f;
		glm::vec3 Q = p0 * t + p1 * u; //Density Interpolation
		Q *= s; // shift to shader ! 
		return Q;
	}


	void SurfaceExtractor::generate_vertex(const ivec3& offsetPos, i32 LOD, long t, u8 v0, u8 v1, const glm::vec3& normal, VoxelMesh* mesh)
	{
		glm::vec3 P0 = (offsetPos + CORNER_INDEXES[v0] * LOD);
		glm::vec3 P1 = (offsetPos + CORNER_INDEXES[v1] * LOD);

		glm::vec3 Q = interpolate_voxel_vector(t, P0, P1);
		mesh->vertices.push_back(VoxelVertex{ Q, normal });
	}

	void SurfaceExtractor::transvoxel(VolumeData& volume_data, const ivec3& min, const VolumeChunk::Size& size, const OCTTREE_LOD LOD, VoxelMesh* mesh)
	{
		assert(LOD > 0 && "LOD must at least be 1");

		for (i32 x = 0; x < size.value; x++)
		{
			for (i32 y = 0; y < size.value; y++)
			{
				for (i32 z = 0; z < size.value; z++)
				{
					ivec3 sample_position(x, y, z);
					polygonize_cell(volume_data, min, sample_position, LOD, mesh);
				}
			}
		}
	}
	
	void SurfaceExtractor::polygonize_cell(VolumeData& volume_data, const ivec3& offset_position, const ivec3& pos, const i32 LOD, VoxelMesh* mesh)
	{
		const ivec3 absolute_position = offset_position + pos * LOD;

		u8 direction_mask = u8((pos.x > 0 ? 1 : 0) | ((pos.z > 0 ? 1 : 0) << 1) | ((pos.y > 0 ? 1 : 0) << 2));

		Sample density[8];

		for (int i = 0; i < 8; i++)
		{
			density[i] = volume_data.get(absolute_position + CORNER_INDEXES[i] * LOD);
		}

		u8 case_code = get_case_code(density);

		if ((case_code ^ ((density[7].value >> 7) & 0xFF)) == 0)
		{
			return;
		}

		glm::vec3 corner_normals[8];
		for (i32 i = 0; i < 8; i++)
		{
			ivec3 p = absolute_position + CORNER_INDEXES[i] * LOD;

			corner_normals[i].x = f32(volume_data.get(p + ivec3(1, 0, 0)).value - volume_data.get(p - ivec3(1, 0, 0)).value) * 0.5f;
			corner_normals[i].y = f32(volume_data.get(p + ivec3(0, 1, 0)).value - volume_data.get(p - ivec3(0, 1, 0)).value) * 0.5f;
			corner_normals[i].z = f32(volume_data.get(p + ivec3(0, 0, 1)).value - volume_data.get(p - ivec3(0, 0, 1)).value) * 0.5f;

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
				generate_vertex(absolute_position, LOD, t, v0, v1, normal, mesh);
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


	u8 SurfaceExtractor::get_case_code(Sample density[8])
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
}








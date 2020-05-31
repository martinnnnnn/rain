#pragma once

#include <vector>
#include <array>
#include <future>
#include <assert.h>
#include <gtx/transform.hpp>

#include "core/core.h"
#include "oct_tree_lod.h"
#include "volume_data.h"
#include "engine/gfx/ogl/ogl_texture.h"
#include "engine/data/data_system.h"

namespace rain::engine::voxel2
{
	struct VoxelVertex
	{
		glm::vec3 vertice;
		glm::vec3 normal;
	};

	struct VoxelMesh
	{
		VoxelMesh()
			: vao(0)
			, vbo(0)
			, ebo(0)
		{

		}
		std::vector<VoxelVertex> vertices;
		std::vector<u32> indices;
		u32 vao;
		u32 vbo;
		u32 ebo;
	};

	struct OctTreeNode
	{
		static const u8 OCTTREE_CHILDREN_COUNT = 8;

		using Children = std::array<OctTreeNode*, OCTTREE_CHILDREN_COUNT>;

		static constexpr std::array<ivec3, OCTTREE_CHILDREN_COUNT> CHILDREN_MINS =
		{
			ivec3(0, 0, 0),
			ivec3(1, 0, 0),
			ivec3(0, 0, 1),
			ivec3(1, 0, 1),
			ivec3(0, 1, 0),
			ivec3(1, 1, 0),
			ivec3(0, 1, 1),
			ivec3(1, 1, 1)
		};

		OctTreeNode(VolumeData& volume_data, const ivec3& min, OCTTREE_LOD lod, i32 size, f32 max_distance);

		void	update(const glm::vec3& other);

		// async
		enum class State
		{
			Initial,

			ChildrenLoadInit,
			ChildrenLoading,
			ChildrenLoaded,
			ChildrenUnloading,

			SurfaceLoadInit,
			SurfaceLoading,
			SurfaceLoaded,
			SurfaceUnloading,
		};
		State state;
		std::future<void> async_result;
		void update_async(const glm::vec3& other);
		void draw_async();

		void	draw();
		void	draw_debug();

	private:

		// obselete
		//void	load_instanting();
		//void	unload_instanciating();

		void	load_transvoxel();
		void	unload_transvoxel();

		ivec3   get_center() const;
		void    create_children();
		void    delete_children();
		void    update_children(const glm::vec3& other);
		bool    point_collision(const glm::vec3& other) const;

		VolumeData&			volume_data;
		const ivec3			min;
		const OCTTREE_LOD	LOD;
		const i32			size;
		const f32			max_distance;
		bool				has_children;
		bool				is_loaded;
		Children			children;

		// transvoxel
		VoxelMesh			voxel_mesh;

		// debug // obselete
		//std::vector<glm::mat4>	mats;
		//u32						vao;
		//u32						vbo_instances;
		//u32						vbo_cube;
		//handle<Texture> const * texture_handle;
	};
}



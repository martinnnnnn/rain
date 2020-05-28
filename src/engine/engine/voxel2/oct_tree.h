#pragma once

#include <vector>
#include <array>
#include "assert.h"
#include "gtx/transform.hpp"

#include "core/core.h"
#include "volume_data.h"
#include "engine/gfx/ogl/ogl_texture.h"
#include "engine/data/data_system.h"

// compute correct max distance

namespace rain::engine::voxel2
{
	using OCTTREE_LOD = i32;

	constexpr OCTTREE_LOD OCTTREE_LOD_1 = 1;
	constexpr OCTTREE_LOD OCTTREE_LOD_2 = OCTTREE_LOD_1 * 2;
	constexpr OCTTREE_LOD OCTTREE_LOD_3 = OCTTREE_LOD_2 * 2;
	constexpr OCTTREE_LOD OCTTREE_LOD_4 = OCTTREE_LOD_3 * 2;
	constexpr OCTTREE_LOD OCTTREE_LOD_5 = OCTTREE_LOD_4 * 2;
	constexpr OCTTREE_LOD OCTTREE_LOD_MIN = OCTTREE_LOD_1;
	constexpr OCTTREE_LOD OCTTREE_LOD_MAX = OCTTREE_LOD_3;

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
		std::vector<u16> indices;
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

		void	draw();
		void	draw_debug();

	private:

		void	load_instanting();
		void	unload_instanciating();

		void	load_transvoxel();
		void	unload_transvoxel();

		ivec3 get_center() const;
		void create_children();
		void delete_children();
		void update_children(const glm::vec3& other);
		bool point_collision(const glm::vec3& other) const;

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

		// debug
		std::vector<glm::mat4>	mats;
		u32						vao;
		u32						vbo_instances;
		u32						vbo_cube;
		handle<Texture> const * texture_handle;
	};

	struct OctTree
	{
		OctTree();

		void init(VolumeData* volume_data, u32 size, OCTTREE_LOD max_lod);
		void update(const glm::vec3& other);
        void draw();
        void draw_debug();

		VolumeData*					volume_data;
		u32							size;
		OCTTREE_LOD					MAX_LOD;
		std::vector<OctTreeNode*>	roots;
	};

	struct SurfaceExtractor
	{

		static void transvoxel(VolumeData& volume_data, const ivec3& min, const VolumeChunk::Size& size, const OCTTREE_LOD LOD, VoxelMesh* mesh);

	private:

		static constexpr ivec3 CORNER_INDEXES[]
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

		static void			polygonize_cell(VolumeData& volume_data, const ivec3& offset_position, const ivec3& pos, const i32 LOD, VoxelMesh* mesh);
		static u8			get_case_code(Sample density[8]);
		static glm::vec3	interpolate_voxel_vector(const long t, const glm::vec3& p0, const glm::vec3& p1);
		static void			generate_vertex(const ivec3& offsetPos, i32 LOD, long t, u8 v0, u8 v1, const glm::vec3& normal, VoxelMesh* mesh);

	};
}



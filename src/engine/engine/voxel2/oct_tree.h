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
	constexpr OCTTREE_LOD OCTTREE_LOD_MAX = OCTTREE_LOD_5;

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

		ivec3 get_center() const;
		bool point_collision(const glm::vec3& other) const;
		void create_children();
		void delete_children();
		void update_children(const glm::vec3& other);
		void load();
		void unload();
		void draw();
        void draw_debug();
		void update(const glm::vec3& other);

		VolumeData&			volume_data;
		const ivec3			min;
		const OCTTREE_LOD	LOD;
		const i32			size;
		const f32			max_distance;
		bool				has_children;
		bool				is_loaded;
		Children			children;

		// debug
		std::vector<glm::mat4> mats;
		u32 vao;
		u32 vbo_instances;
		u32 vbo_cube;
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
}



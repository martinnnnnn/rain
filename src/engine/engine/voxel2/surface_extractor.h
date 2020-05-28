#pragma once

#include <vector>
#include <array>
#include "assert.h"
#include <gtx/transform.hpp>
#include <glm.hpp>

#include "core/core.h"
#include "oct_tree_lod.h"
#include "volume_size.h"
#include "volume_data.h"

namespace rain::engine::voxel2
{
    struct VoxelMesh;

	struct SurfaceExtractor
	{
		static void transvoxel(VolumeData& volume_data, const ivec3& min, const VolumeSize& size, const OCTTREE_LOD LOD, VoxelMesh* mesh);

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



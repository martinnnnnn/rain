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

	namespace SurfaceExtractor
	{
		void transvoxel(VolumeData& volume_data, const ivec3& min, const VolumeSize& size, const OCTTREE_LOD LOD, VoxelMesh* mesh);
	};
}
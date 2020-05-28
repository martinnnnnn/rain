#pragma once

#include "core/core.h"

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
}



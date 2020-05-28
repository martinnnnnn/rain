#pragma once

#include <vector>
#include <array>
#include "assert.h"
#include "gtx/transform.hpp"

#include "core/core.h"
#include "volume_data.h"
#include "oct_tree_lod.h"

namespace rain::engine::voxel2
{
    struct OctTreeNode;

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



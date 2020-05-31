#include  "oct_tree.h"


#include "engine/gfx/ogl/ogl_renderer.h"
#include "engine/core/context.h"
#include "engine/core/profiler.h"
#include "engine/game/world.h"
#include "oct_tree_node.h"

namespace rain::engine::voxel2
{
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
			roots[u]->update_async(other);
		}
	}

	void OctTree::draw()
	{
		for (u32 u(0); u < roots.size(); ++u)
		{
			roots[u]->draw_async();
		}
	}

    void OctTree::draw_debug()
    {
        for (u32 u(0); u < roots.size(); ++u)
        {
            roots[u]->draw_debug();
        }
    }
}








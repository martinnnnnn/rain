#include "oct_tree.h"

#include "engine/gfx/ogl/ogl_renderer.h"
#include "engine/core/context.h"

namespace rain::engine::voxel
{
    //void build_oct_tree(oct_tree* tree, const glm::vec3 min, const f32 LOD, oct_tree* parent)
    //{
    //    tree->min = min;
    //    tree->parent = parent;
    //    tree->LOD = LOD;

    //    if (!core::almost_equal(tree->LOD, OCTTREE_LOD_0))
    //    {
    //        for (u8 i = 0; i < OCTTREE_CHILDREN_COUNT; ++i)
    //        {
    //            tree->children[i] = new oct_tree();
    //        }

    //        const glm::vec3 center = min + (LOD / 2.0f);

    //        build_oct_tree(tree->children[0], glm::vec3{ min.x,    min.y,    min.z }, tree->LOD / 2.0f, tree);
    //        build_oct_tree(tree->children[1], glm::vec3{ center.x, min.y,    min.z }, tree->LOD / 2.0f, tree);
    //        build_oct_tree(tree->children[2], glm::vec3{ min.x,    min.y,    center.z }, tree->LOD / 2.0f, tree);
    //        build_oct_tree(tree->children[3], glm::vec3{ center.x, min.y,    center.z }, tree->LOD / 2.0f, tree);
    //        build_oct_tree(tree->children[4], glm::vec3{ min.x,    center.y, min.z }, tree->LOD / 2.0f, tree);
    //        build_oct_tree(tree->children[5], glm::vec3{ center.x, center.y, min.z }, tree->LOD / 2.0f, tree);
    //        build_oct_tree(tree->children[6], glm::vec3{ min.x,    center.y, center.z }, tree->LOD / 2.0f, tree);
    //        build_oct_tree(tree->children[7], glm::vec3{ center.x, center.y, center.z }, tree->LOD / 2.0f, tree);
    //    }
    //}

    //void free_oct_tree(oct_tree* tree)
    //{
    //    for (u8 i = 0; i < OCTTREE_CHILDREN_COUNT; ++i)
    //    {
    //        if (tree->children[i])
    //        {
    //            free_oct_tree(tree->children[i]);
    //            delete tree->children[i];
    //        }
    //    }
    //}

    //void draw_oct_tree(oct_tree* tree)
    //{
    //    for (u8 i = 0; i < OCTTREE_CHILDREN_COUNT; ++i)
    //    {
    //        if (tree->children[i])
    //        {
    //            draw_oct_tree(tree->children[i]);
    //        }
    //    }

    //    const glm::vec3 center = tree->min + (tree->LOD / 2.0f);
    //    glm::vec3 color{};

    //    if (core::almost_equal(tree->LOD, OCTTREE_LOD_0))
    //    {
    //        color = glm::vec3(1, 0, 0);
    //    }
    //    if (core::almost_equal(tree->LOD, OCTTREE_LOD_1))
    //    {
    //        color = glm::vec3(0, 1, 0);
    //    }
    //    if (core::almost_equal(tree->LOD, OCTTREE_LOD_2))
    //    {
    //        color = glm::vec3(0, 0, 1);
    //    }
    //    if (core::almost_equal(tree->LOD, OCTTREE_LOD_3))
    //    {
    //        color = glm::vec3(0.5f, 0.5f, 0);
    //    }

    //    RAIN_RENDERER->draw_debug_cube(center, tree->LOD, tree->LOD, color);
    //}
}
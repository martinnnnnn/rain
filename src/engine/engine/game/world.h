#pragma once

#include <string>

#include "core/core.h"
#include "engine/data/geometry/mesh.h"
#include "engine/network/client.h"
#include "scene_graph.h"
#include "engine/voxel/voxel.h"
#include "engine/voxel/oct_tree.h"
#include "engine/voxel2/volume_data.h"
#include "engine/voxel2/oct_tree.h"

namespace rain::engine
{
    struct Camera;
    struct voxel_map;

    struct MainCamera
    {
        Camera* camera;
        core::transform* transform;
    };

    struct character
    {
        glm::vec3 position;
        f32 speed;
    };

    struct World
    {
        std::string name;
		std::string file_name;

        MainCamera main_camera;

        void init(const std::string& path);

        void update_camera(const float _deltaTime);
        void update_physics(const float _deltaTime);
        void draw(const float _alpha);

        // TEMP
        scene_graph sg;

        u32 vao_quickhull;

        voxel_map* vmap;

        voxel::vox_map voxmap;
        character chara;
        //voxel::oct_tree octtree;
		voxel2::VolumeData volume_data;
		voxel2::OctTree    oct_tree;
    };
}


#pragma once

#include <string>

#include "core/core.h"
#include "engine/data/geometry/mesh.h"
#include "engine/network/client.h"
#include "scene_graph.h"
#include "engine/voxel/voxel.h"

namespace rain::engine
{
    struct Camera;
    struct voxel_map;

    struct MainCamera
    {
        Camera* camera;
        core::transform* transform;
    };

    struct World
    {
        std::string name;
        core::File file;

        MainCamera main_camera;

        void init(const std::string& _path);

        void update_camera(const float _deltaTime);
        void update_physics(const float _deltaTime);
        void draw(const float _alpha);

        // TEMP
        scene_graph sg;

        u32 vao_quickhull;

        voxel_map* vmap;

        //u32 vao_transvoxel;

        //std::vector<glm::vec3> transVertices;
        //std::vector<glm::vec3> transNormals;

        isosurface::tvox_map tmap;
    };
}


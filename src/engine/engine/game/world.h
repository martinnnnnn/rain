#pragma once

#include <string>
#include <entt.hpp>

#include "core/core.h"
#include "engine/data/geometry/mesh.h"
#include "engine/network/client.h"

#include "engine/voxel/voxel.h"

namespace rain::engine
{
    struct Camera;

    struct MainCamera
    {
        Camera* camera;
        core::transform* transform;
    };

    struct World
    {
        std::string name;
        entt::DefaultRegistry registry;
        core::File file;

        //MainCamera main_camera;
        u32 main_camera_id;

        void init(const std::string& _path);

        void update_camera(const float _deltaTime);
        void update_physics(const float _deltaTime);
        void draw(const float _alpha);

        Camera& main_camera();
        core::transform& main_camera_transform();

        // TEMP
        u32 temp_vao;
        u32 temp_vao2;
        voxel_map* vmap;
    };
}


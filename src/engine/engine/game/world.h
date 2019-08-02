#pragma once

#include <string>
#include <entt.hpp>

#include "core/core.h"
#include "engine/data/geometry/mesh.h"
#include "engine/network/client.h"



namespace rain::engine
{

    //bool visible_face_XN(Chunk& chunk, u32 i, u32 j, u32 k);
    //bool visible_face_XP(Chunk& chunk, u32 i, u32 j, u32 k);
    //void generate_mesh(Chunk& chunk); 
    //void create_run(Chunk& chunk, Block* b, u32 i, u32 j, u32 k, u32 access);


    struct World
    {
        std::string name;
        entt::DefaultRegistry registry;
        core::File file;

        void init(const std::string& _path);

        void update_camera(const float _deltaTime);
        void update_physics(const float _deltaTime);
        void draw(const float _alpha);

        // TEMP
        u32 temp_vao;
        core::voxel_map* vmap;
    };
}


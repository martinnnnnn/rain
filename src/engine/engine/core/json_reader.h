#pragma once


#include <string.h>
#include <rapidjson/document.h>

#include "core/core.h"
#include "math/math.h"
#include "engine/game/world.h"
#include "engine/physics/collision.h"
#include "engine/physics/rigid_body.h"
#include "engine/physics/spring.h"
#include "engine/core/config.h"
#include "engine/data/geometry/mesh.h"
#include "engine/data/data_system.h"
#include "engine/data/material/material.h"
#include "engine/gfx/gfx_camera.h"

namespace rain::engine
{

    namespace JsonReader
    {
        std::string get_string(const rapidjson::Value& _json_value);
        math::vec3 read_vec3(const rapidjson::Value& _json);
        math::quat read_quat(const rapidjson::Value& _json);
        RigidBody read_rigid_body(const rapidjson::Value& _json);
        math::Sphere read_bounding_sphere(const rapidjson::Value& _json);
        Spring read_spring2(const rapidjson::Value& _json);
        math::Plane read_plane(const rapidjson::Value& _json);
        void read_transform(const rapidjson::Value& _json, math::Transform& _transform);
        void read_model(const rapidjson::Value& _json, Model& _model);
        void read_mesh_bound(const rapidjson::Value& _json, const Model& _model, MeshBound& _meshBound);
        void read_material(const rapidjson::Value& _json, Material& _material);
        void read_camera(const rapidjson::Value& _json, Camera& _camera);
        void read_world(const std::string& _json, World& _world);
        void read_config(const std::string& _path, Config& _config);
        //void read_shaders_info(const std::string& _path, std::vector<ShadersInfo>& _info);
    };
}
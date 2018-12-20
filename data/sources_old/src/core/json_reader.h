#pragma once


#include <string.h>
#include <rapidjson/document.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "game/world.h"
#include "math/transform.h"
#include "physics/collision.h"
#include "physics/rigid_body.h"
#include "physics/spring.h"
#include "core/config.h"
#include "data/geometry/mesh.h"
#include "data/data_system.h"
#include "data/material/material.h"

namespace rain
{
    namespace JsonReader
    {
        std::string get_string(const rapidjson::Value& _json_value);
        glm::vec3 read_vec3(const rapidjson::Value& _json);
        glm::quat read_quat(const rapidjson::Value& _json);
        Transform read_transform(const rapidjson::Value& _json);
        RigidBody read_rigid_body(const rapidjson::Value& _json);
        Sphere read_bounding_sphere(const rapidjson::Value& _json);
        Spring read_spring2(const rapidjson::Value& _json);
        Plane read_plane(const rapidjson::Value& _json);
        void read_model(const rapidjson::Value& _json, Model& _model);
        void read_mesh_bound(const rapidjson::Value& _json, const Model& _model, MeshBound& _meshBound);
        void read_material(const rapidjson::Value& _json, Material& _material);
        void read_world(const std::string& _json, World& _world);
        void read_config(const std::string& _path, Config& _config);
        //void read_shaders_info(const std::string& _path, std::vector<ShadersInfo>& _info);
    };
}
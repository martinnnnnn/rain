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
#include "core/data_indexer.h"

namespace rain
{
    namespace JsonReader
    {
        std::string get_string(const rapidjson::Value& _json_value);
        glm::vec3 read_vec3(const rapidjson::Value& _json);
        glm::quat read_quat(const rapidjson::Value& _json);
        Transform read_transform(const rapidjson::Value& _json);
        RigidBody read_rigid_body(const rapidjson::Value& _json);
        BoundingSphere read_bounding_sphere(const rapidjson::Value& _json);
        Spring read_spring(const rapidjson::Value& _json);
        SpringRope read_spring_rope(const rapidjson::Value& _json);
        void read_world(const std::string& _json, World& _world);
        void read_config(const std::string& _path, Config& _config);
    };
}
#pragma once


#include <string.h>
#include <rapidjson/document.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "game/world.h"
#include "math/transform.h"
#include "physics/collision.h"
#include "physics/rigid_body.h"
#define JSON_ASSEZR

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
        void read_world(World& world, const std::string& _json);
    };
}
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
    class JsonReader
    {
    public:

        static std::string get_string(const rapidjson::Value& _json_value);
        static glm::vec3 read_vec3(const rapidjson::Value& _json);
        static glm::quat read_quat(const rapidjson::Value& _json);
        static Transform read_transform(const rapidjson::Value& _json);
        static RigidBody read_rigid_body(const rapidjson::Value& _json);
        static BoundingSphere read_bounding_sphere(const rapidjson::Value& _json);
        static void read_world(World& world, const std::string& _json);
    };
}
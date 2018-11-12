#pragma once

#include "physics/rigid_body.h"

#include <nlohmann/json.hpp>
#include <glm/glm.hpp>


namespace nlohmann
{
    template <>
    struct adl_serializer<glm::vec2>
    {
        static void to_json(json& j, const glm::vec2& vect)
        {
            j = json{ vect.x, vect.y };
        }

        static void from_json(const json& j, glm::vec2& vect)
        {
            vect.x = j[0].get<float>();
            vect.y = j[1].get<float>();
        }
    };

    template <>
    struct adl_serializer<glm::vec3>
    {
        static void to_json(json& j, const glm::vec3& vect)
        {
            j = json{ vect.x, vect.y, vect.z };
        }

        static void from_json(const json& j, glm::vec3& vect)
        {
            vect.x = j[0].get<float>();
            vect.y = j[1].get<float>();
            vect.z = j[2].get<float>();
        }
    };

    template <>
    struct adl_serializer<glm::mat4>
    {
        static void to_json(json& j, const glm::mat4& m)
        {
            j = json
            {
                m[0][0] , m[0][1], m[0][2], m[0][3],
                m[1][0] , m[1][1], m[1][2], m[1][3],
                m[2][0] , m[2][1], m[2][2], m[2][3],
                m[3][0] , m[3][1], m[3][2], m[3][3]
            };
        }

        static void from_json(const json& j, glm::mat4& m)
        {
            m[0][0] = j[0].get<float>();
            m[0][1] = j[1].get<float>();
            m[0][2] = j[2].get<float>();
            m[0][3] = j[3].get<float>();

            m[1][0] = j[4].get<float>();
            m[1][1] = j[5].get<float>();
            m[1][2] = j[6].get<float>();
            m[1][3] = j[7].get<float>();

            m[2][0] = j[8].get<float>();
            m[2][1] = j[9].get<float>();
            m[2][2] = j[10].get<float>();
            m[2][3] = j[11].get<float>();

            m[3][0] = j[12].get<float>();
            m[3][1] = j[13].get<float>();
            m[3][2] = j[14].get<float>();
            m[3][3] = j[15].get<float>();
        }
    };
}


namespace rain
{
    using json = nlohmann::json;

    void to_json(json& j, const RigidBody& _body)
    {
        //j = json
        //{
        //    {"name", _body.name},
        //    {"address", _body.address},
        //    {"age", _body.age}
        //};
    }

    void from_json(const json& j, RigidBody& p)
    {
        
        j.at("name").get_to("");
        j.at("address").get_to(p.address);
        j.at("age").get_to(p.age);
    }
}
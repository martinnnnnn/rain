#include "json_reader.h"


#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include "core/logger.h"

namespace rain
{
    std::string JsonReader::get_string(const rapidjson::Value& _json_value)
    {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        _json_value.Accept(writer);
        return buffer.GetString();
    }
    
    void JsonReader::read_config(const std::string& _json, Config& _config)
    {
        rapidjson::Document config_document;
        config_document.Parse(_json.c_str());

        _config.engine_name = config_document["engine_name"].GetString();
        RAIN_LOG("Reading config file for engine : %s\n", _config.engine_name.c_str());

        _config.data_root = config_document["data_root"].GetString();
        _config.starting_world = config_document["starting_world"].GetString();
        _config.screen_width = config_document["screen_width"].GetUint();
        _config.screen_height = config_document["screen_height"].GetUint();
        _config.full_screen = config_document["full_screen"].GetBool();
    }

    void JsonReader::read_world(const std::string& _json, World& _world)
    {
        rapidjson::Document world_document;
        world_document.Parse(_json.c_str());

        _world.name = world_document["name"].GetString();
        RAIN_LOG("parsing world : %s\n", _world.name.c_str());

        const rapidjson::Value& world_objects = world_document["objects"];
        entt::DefaultRegistry& registry = _world.registry;
        for (u32 i = 0; i < world_objects.Size(); i++)
        {
            const rapidjson::Value& world_object = world_objects[i];

            auto entity = registry.create();

            // TODO (martin) : should really be a struct idenfier there
            u32& id = registry.assign<u32>(entity);
            id = world_object["id"].GetUint();

            if (world_object.HasMember("Transform"))
            {
                Transform& transform = registry.assign<Transform>(entity);
                transform = read_transform(world_object["Transform"]);
            }
            if (world_object.HasMember("RigidBody"))
            {
                RigidBody& body = registry.assign<RigidBody>(entity);
                body = read_rigid_body(world_object["RigidBody"]);
            }
            if (world_object.HasMember("BoundingSphere"))
            {
                Sphere& sphere = registry.assign<Sphere>(entity);
                sphere = read_bounding_sphere(world_object["BoundingSphere"]);
            }
            if (world_object.HasMember("Spring"))
            {
                Spring& spring = registry.assign<Spring>(entity);
                spring = read_spring2(world_object["Spring"]);
            }
            if (world_object.HasMember("BoundingPlane"))
            {
                Plane& plane = registry.assign<Plane>(entity);
                plane = read_plane(world_object["BoundingPlane"]);
                printf("hello");
            }
        }
    }

    glm::vec3 JsonReader::read_vec3(const rapidjson::Value& _json)
    {
        glm::vec3 position;
        for (u32 i = 0; i < _json.Size(); i++)
        {
            position[i] = _json[i].GetFloat();
        }

        return position;
    }

    glm::quat JsonReader::read_quat(const rapidjson::Value& _json)
    {
        glm::quat orientation;
        for (u32 i = 0; i < _json.Size(); i++)
        {
            orientation[i] = _json[i].GetFloat();
        }

        return orientation;
    }

    Transform JsonReader::read_transform(const rapidjson::Value& _json)
    {
        Transform transform;
        if (_json.HasMember("position"))
        {
            transform.position = read_vec3(_json["position"]);
        }
        if (_json.HasMember("orientation"))
        {
            transform.orientation = read_quat(_json["orientation"]);
        }
        return transform;
    }

    RigidBody JsonReader::read_rigid_body(const rapidjson::Value& _json)
    {
        RigidBody body;

        if (_json.HasMember("mass"))
        {
            body.mass = _json["mass"].GetFloat();
            body.mass_inverse = 1.0f / body.mass;
        }
        if (_json.HasMember("size"))
        {
            body.size = _json["size"].GetFloat();
        }
        if (_json.HasMember("momentum"))
        {
            body.momentum = read_vec3(_json["momentum"]);
        }
        if (_json.HasMember("angularMomentum"))
        {
            body.angularMomentum = read_vec3(_json["angularMomentum"]);
        }
        if (_json.HasMember("rotationInertia"))
        {
            if (!strcmp(_json["rotationInertia"].GetString(), "cube"))
            {
                body.rotationInertia = (1.0f / 6.0f) * body.mass * powf(body.size, 2);;
                body.rotationInertiaInverse = 1.0f / body.rotationInertia;
            }
        }
        if (_json.HasMember("applygravity"))
        {
            body.applyGravity = _json["applygravity"].GetBool();
        }
        if (_json.HasMember("infiniteMass"))
        {
            body.infiniteMass = _json["infiniteMass"].GetBool();
        }

        return body;
    }

    Sphere JsonReader::read_bounding_sphere(const rapidjson::Value& _json)
    {
        Sphere bound;

        if (_json.HasMember("offset"))
        {
            bound.offset = read_vec3(_json["offset"]);
        }
        if (_json.HasMember("radius"))
        {
            bound.radius = _json["radius"].GetFloat();
        }

        return bound;
    }


    Spring JsonReader::read_spring2(const rapidjson::Value& _json)
    {
        Spring spring;

        if (_json.HasMember("entityA"))
        {
            spring.entityA = _json["entityA"].GetUint();
        }
        if (_json.HasMember("entityB"))
        {
            spring.entityB = _json["entityB"].GetUint();
        }
        if (_json.HasMember("anchorPointA"))
        {
            spring.anchorPointA = read_vec3(_json["anchorPointA"]);
        }
        if (_json.HasMember("anchorPointB"))
        {
            spring.anchorPointB = read_vec3(_json["anchorPointB"]);
        }
        if (_json.HasMember("distance"))
        {
            spring.distance = _json["distance"].GetFloat();
        }
        if (_json.HasMember("k"))
        {
            spring.k = _json["k"].GetFloat();
        }
        if (_json.HasMember("b"))
        {
            spring.b = _json["b"].GetFloat();
        }

        return spring;
    }

    Plane JsonReader::read_plane(const rapidjson::Value& _json)
    {
        glm::vec3 position {};
        glm::vec3 normal {};

        glm::vec3 point1 {};
        glm::vec3 point2 {};
        glm::vec3 point3 {};

        if (_json.HasMember("position") && _json.HasMember("normal"))
        {
            position = read_vec3(_json["position"]);
            normal = read_vec3(_json["normal"]);
            auto hello = Plane(position, normal);
            return Plane(position, normal);
        }
        else if (_json.HasMember("point1") && _json.HasMember("point2") && _json.HasMember("point3"))
        {
            point1 = read_vec3(_json["point1"]);
            point2 = read_vec3(_json["point2"]);
            point3 = read_vec3(_json["point3"]);
            return Plane(point1, point2, point3);
        }
        return Plane(glm::vec3{}, glm::vec3{});
    }
}
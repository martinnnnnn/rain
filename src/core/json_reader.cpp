#include "json_reader.h"




namespace rain
{
    std::string JsonReader::get_string(const rapidjson::Value& _json_value)
    {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        _json_value.Accept(writer);
        return buffer.GetString();
    }

    void JsonReader::read_world(World& world, const std::string& _json)
    {
        rapidjson::Document world_document;
        world_document.Parse(_json.c_str());

        world.name = world_document["name"].GetString();
        char buffer[128];
        sprintf_s(buffer, 128, "parsing world : %s\n", world.name.c_str());
        OutputDebugString(buffer);

        const rapidjson::Value& world_objects = world_document["objects"];
        entt::DefaultRegistry& registry = world.registry;
        for (u32 i = 0; i < world_objects.Size(); i++)
        {
            const rapidjson::Value& world_object = world_objects[i];

            auto entity = registry.create();

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
                BoundingSphere& sphere = registry.assign<BoundingSphere>(entity);
                sphere = read_bounding_sphere(world_object["BoundingSphere"]);
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
            transform.currentOrientation = read_quat(_json["orientation"]);
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

    BoundingSphere JsonReader::read_bounding_sphere(const rapidjson::Value& _json)
    {
        BoundingSphere bound;

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

}
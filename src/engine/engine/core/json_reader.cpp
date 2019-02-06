#include "json_reader.h"


#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <nlohmann/json.hpp>

#include "engine/core/context.h"
#include "engine/core/config.h"
#include "engine/data/data_system.h"
#include "engine/gfx/ogl/ogl_renderer.h"

namespace rain::engine::json_reader
{
    using namespace rain::math;

    std::string get_string(const rapidjson::Value& _json_value)
    {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        _json_value.Accept(writer);
        return buffer.GetString();
    }
    
    void read_config(const std::string& _json, Config& _config)
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

    void read_world(const std::string& _json, World& _world)
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

            u32& id = registry.assign<u32>(entity);
            id = world_object["id"].GetUint();

            if (world_object.HasMember("Transform"))
            {
                transform& t = registry.assign<transform>(entity);
                read_transform(world_object["Transform"], t);
                RAIN_LOG_RAW("%s\n", json_writer::serialize(t).c_str());
            }
            if (world_object.HasMember("RigidBody"))
            {
                RigidBody& rigid_body = registry.assign<RigidBody>(entity);
                read_rigid_body(world_object["RigidBody"], rigid_body);
            }
            if (world_object.HasMember("Sphere"))
            {
                sphere& s = registry.assign<sphere>(entity);
                read_sphere(world_object["Sphere"], s);
            }
            if (world_object.HasMember("Spring"))
            {
                Spring& spring = registry.assign<Spring>(entity);
                read_spring(world_object["Spring"], spring);
            }
            if (world_object.HasMember("Plane"))
            {
                plane& p = registry.assign<plane>(entity);
                read_plane(world_object["Plane"], p);
            }
            if (world_object.HasMember("Model"))
            {
                Model& model = registry.assign<Model>(entity);
                read_model(world_object["Model"], model);

                if (world_object.HasMember("MeshBound"))
                {
                    MeshBound& meshBound = registry.assign<MeshBound>(entity);
                    read_mesh_bound(world_object["MeshBound"], model, meshBound);
                }
            }
            if (world_object.HasMember("Material"))
            {
                Material& material = registry.assign<Material>(entity);
                read_material(world_object["Material"], material);
            }
            if (world_object.HasMember("Camera"))
            {
                Camera& camera = registry.assign<Camera>(entity);
                read_camera(world_object["Camera"], camera);
            }
        }
    }

    void read_camera(const rapidjson::Value& _json, Camera& _camera)
    {
        if (_json.HasMember("movement_speed"))
        {
            _camera.movement_speed = _json["movement_speed"].GetFloat();
        }
    }

    void read_vec3(const rapidjson::Value& _json, math::vec3& vec)
    {
        for (u32 i = 0; i < _json.Size(); i++)
        {
            vec[i] = _json[i].GetFloat();
        }
    }

    void read_quat(const rapidjson::Value& _json, math::quat& q)
    {
        for (u32 i = 0; i < _json.Size(); i++)
        {
            q[i] = _json[i].GetFloat();
        }
    }

    void read_transform(const rapidjson::Value& _json, transform& _transform)
    {
        read_transform(get_string(_json), _transform);

        _transform.lastPosition = _transform.position;
        _transform.lastOrientation = _transform.orientation;
        _transform.lastScale = _transform.scale;

        //if (_json.HasMember("position"))
        //{
        //     read_vec3(_json["position"], _transform.position);
        //    _transform.lastPosition = _transform.position;
        //}
        //if (_json.HasMember("orientation"))
        //{
        //    if (_json["orientation"].Size() == 4)
        //    {
        //         read_quat(_json["orientation"], _transform.orientation);
        //    }
        //    else if (_json["orientation"].Size() == 3)
        //    {
        //        vec3 v{};
        //        read_vec3(_json["orientation"], v);
        //        _transform.orientation = from_euler(v);
        //    }
        //    _transform.lastOrientation = _transform.orientation;
        //}
        //if (_json.HasMember("scale"))
        //{
        //     read_vec3(_json["scale"], _transform.scale);
        //    _transform.lastScale = _transform.scale;
        //}
    }

    void read_rigid_body(const rapidjson::Value& _json, RigidBody& rigid_body)
    {
        if (_json.HasMember("mass"))
        {
            rigid_body.mass = _json["mass"].GetFloat();
            rigid_body.mass_inverse = 1.0f / rigid_body.mass;
        }
        if (_json.HasMember("size"))
        {
            rigid_body.size = _json["size"].GetFloat();
        }
        if (_json.HasMember("momentum"))
        {
            read_vec3(_json["momentum"], rigid_body.momentum);
        }
        if (_json.HasMember("angularMomentum"))
        {
             read_vec3(_json["angularMomentum"], rigid_body.angularMomentum);
        }
        if (_json.HasMember("rotationInertia"))
        {
            if (!strcmp(_json["rotationInertia"].GetString(), "cube"))
            {
                rigid_body.rotationInertia = (1.0f / 6.0f) * rigid_body.mass * powf(rigid_body.size, 2);;
                rigid_body.rotationInertiaInverse = 1.0f / rigid_body.rotationInertia;
            }
        }
        if (_json.HasMember("applygravity"))
        {
            rigid_body.applyGravity = _json["applygravity"].GetBool();
        }
        if (_json.HasMember("infiniteMass"))
        {
            rigid_body.infiniteMass = _json["infiniteMass"].GetBool();
        }
    }

    void read_sphere(const rapidjson::Value& _json, math::sphere& sphere)
    {
        if (_json.HasMember("offset"))
        {
             read_vec3(_json["offset"], sphere.offset);
        }
        if (_json.HasMember("radius"))
        {
            sphere.radius = _json["radius"].GetFloat();
        }
    }


    void read_spring(const rapidjson::Value& _json, Spring& spring)
    {
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
            read_vec3(_json["anchorPointA"], spring.anchorPointA);
        }
        if (_json.HasMember("anchorPointB"))
        {
             read_vec3(_json["anchorPointB"], spring.anchorPointB);
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
    }

    void read_plane(const rapidjson::Value& _json, math::plane& p)
    {
        vec3 position {};
        vec3 normal {};
        
        vec3 point1 {};
        vec3 point2 {};
        vec3 point3 {};

        if (_json.HasMember("position") && _json.HasMember("normal"))
        {
            read_vec3(_json["position"], position);
            read_vec3(_json["normal"], normal);
            p = plane(position, normal);
        }
        else if (_json.HasMember("point1") && _json.HasMember("point2") && _json.HasMember("point3"))
        {
            read_vec3(_json["point1"], point1);
            read_vec3(_json["point2"], point2);
            read_vec3(_json["point3"], point3);
            p = plane(point1, point2, point3);
        }
    }

    void read_model(const rapidjson::Value& _json, Model& _model)
    {
        if (_json.HasMember("path"))
        {
            _model.path = file_path(RAIN_CONFIG->data_root + std::string(_json["path"].GetString()));
            _model.mesh = RAIN_FIND_DATA_FROM_PATH(Mesh, _model.path.get_path_absolute());
            //assert(_model.mesh);
            RAIN_RENDERER->load_mesh(_model.mesh->data);
        }
    }

    void read_mesh_bound(const rapidjson::Value& _json, const Model& _model, MeshBound& _meshBound)
    {
        _meshBound.points.resize(_model.mesh->data->vertices.size());

        for (u32 i = 0; i < _model.mesh->data->vertices.size(); ++i)
        {
            _meshBound.points[i] = _model.mesh->data->vertices[i].position;
        }
    }

    void read_material(const rapidjson::Value& _json, Material& _material)
    {
        std::string vertex_path;
        std::string fragment_path;
        std::string geometry_path;

        if (_json.HasMember("vertex"))
        {
            vertex_path = RAIN_CONFIG->data_root + _json["vertex"].GetString();
        }
        if (_json.HasMember("fragment"))
        {
            fragment_path = RAIN_CONFIG->data_root + _json["fragment"].GetString();
        }
        if (_json.HasMember("geometry"))
        {
            geometry_path = RAIN_CONFIG->data_root + _json["geometry"].GetString();
        }

        _material.shader.load(vertex_path, fragment_path, geometry_path);
    }

    //void read_shaders_info(const std::string& _json, std::vector<ShadersInfo>& _info)
    //{
    //    rapidjson::Document shaders_document;
    //    shaders_document.Parse(_json.c_str());

    //    auto info_json = shaders_document["shaders_info"].GetArray();
    //    _info.resize(info_json.Size());

    //    for (u32 i = 0; i < info_json.Size(); ++i)
    //    {
    //        if (info_json[i].HasMember("vertex"))
    //        {
    //            _info[i].vertex_path = info_json[i]["vertex"].GetString();
    //        }
    //        if (info_json[i].HasMember("fragment"))
    //        {
    //            _info[i].fragment_path = info_json[i]["fragment"].GetString();
    //        }
    //        if (info_json[i].HasMember("geometry"))
    //        {
    //            _info[i].geometry_path = info_json[i]["geometry"].GetString();
    //        }
    //    }
    //}

    void read_transform(const std::string & json_str, math::transform& t)
    {
        nlohmann::json j = nlohmann::json::parse(json_str);

		for (u32 i = 0; i < j["position"].size(); ++i)
		{
			t.position[i] = j["position"][i];
		}
		for (u32 i = 0; i < j["orientation"].size(); ++i)
		{
			t.orientation[i] = j["orientation"][i];
		}
		for (u32 i = 0; i < j["scale"].size(); ++i)
		{
			t.scale[i] = j["scale"][i];
		}
    }
}

namespace rain::engine::json_writer
{
    std::string serialize(const math::transform& t)
    {
        rapidjson::Document d;
        rapidjson::Value json_transform(rapidjson::kObjectType);
        {
            rapidjson::Value json_position(rapidjson::kArrayType);
            json_position.PushBack(t.position.x, d.GetAllocator());
            json_position.PushBack(t.position.y, d.GetAllocator());
            json_position.PushBack(t.position.z, d.GetAllocator());

            rapidjson::Value json_orientation(rapidjson::kArrayType);
            json_orientation.PushBack(t.orientation.x, d.GetAllocator());
            json_orientation.PushBack(t.orientation.y, d.GetAllocator());
            json_orientation.PushBack(t.orientation.z, d.GetAllocator());
            json_orientation.PushBack(t.orientation.w, d.GetAllocator());

            rapidjson::Value json_scale(rapidjson::kArrayType);
            json_scale.PushBack(t.scale.x, d.GetAllocator());
            json_scale.PushBack(t.scale.y, d.GetAllocator());
            json_scale.PushBack(t.scale.z, d.GetAllocator());
            
            json_transform.AddMember("position", json_position, d.GetAllocator());
            json_transform.AddMember("orientation", json_orientation, d.GetAllocator());
            json_transform.AddMember("scale", json_scale, d.GetAllocator());
        }

        rapidjson::StringBuffer buffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        json_transform.Accept(writer);
        return std::string(buffer.GetString());
    }
}
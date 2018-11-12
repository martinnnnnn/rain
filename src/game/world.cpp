#include "world.h"

#include <cassert>

#include "core/json_reader.h"


namespace rain
{



    void World::init(const std::string& _path)
    {
        //using namespace rapidjson;

        File world_file;
        world_file.open(_path);

        rapidjson::Document world_document;
        world_document.Parse(world_file.read().c_str());
        assert(world_document.IsObject());
        assert(world_document.HasMember("name"));
        assert(world_document["name"].IsString());
        char buffer[128];
        sprintf_s(buffer, 128, "world name : %s\n", world_document["name"].GetString());
        OutputDebugString(buffer);

        const rapidjson::Value& a = world_document["objects"];
        assert(a.IsArray());
        for (u32 i = 0; i < a.Size(); i++)
        {
            const rapidjson::Value& world_object = a[i];
            assert(world_object.IsObject());
            assert(world_object.HasMember("name"));
            assert(world_object["name"].IsString());

            sprintf_s(buffer, 128, "object name : %s\n", world_object["name"].GetString());
            OutputDebugString(buffer);
        }
    }


    //using json = nlohmann::json;

    //void to_json(json& j, const World& _body)
    //{
    //    j = json
    //    {
    //        {"name", _body.name},
    //        {"address", _body.address},
    //        {"age", _body.age}
    //    };
    //}
    //void from_json(const json& j, World& p)
    //{
    //    j.at("name").get_to(p.name);
    //    //j.at("address").get_to(p.address);
    //    //j.at("age").get_to(p.age);
    //}

    //void World::init(const std::string& _path)
    //{
    //    File world_file;
    //    world_file.open(_path);
    //    json world_json = json::parse(world_file.read());
    //    std::string world_name = world_json["name"];

    //    json world_objects = world_json["objects"];
    //    for (size_t i = 0; i < world_objects.size(); ++i)
    //    {
    //        json obj = world_objects[i];
    //        std::string name = obj.get<std::string>("name");
    //        char buffer[128];
    //        sprintf_s(buffer, 128, "object : %s\n", );
    //        OutputDebugString(buffer);
    //    }
    //}




}
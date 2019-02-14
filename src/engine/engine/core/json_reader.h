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
#include "engine/ui/text_field.h"

namespace rain::engine
{

    namespace json_reader
    {
        std::string get_string(const rapidjson::Value& _json_value);
        void read_vec3(const rapidjson::Value& _json, math::vec3& vec);
        void read_vec4(const rapidjson::Value& _json, math::vec4& vec);
        void read_quat(const rapidjson::Value& _json, math::quat& quat);
        void read_rigid_body(const rapidjson::Value& _json, RigidBody& rigid_body);
        void read_sphere(const rapidjson::Value& _json, math::sphere& sphere);
        void read_spring(const rapidjson::Value& _json, Spring& spring);
        void read_plane(const rapidjson::Value& _json, math::plane& plane);
        void read_transform(const rapidjson::Value& _json, math::transform& _transform);
        void read_model(const rapidjson::Value& _json, Model& _model);
        void read_mesh_bound(const rapidjson::Value& _json, const Model& _model, MeshBound& _meshBound);
        void read_material(const rapidjson::Value& _json, Material& _material);
        void read_camera(const rapidjson::Value& _json, Camera& _camera);
        void read_world(const std::string& _json, World& _world);
        void read_config(const std::string& _path, Config& _config);
        void read_text_field(const rapidjson::Value& _json, ui::text_field& field);
        void read_text_list(const rapidjson::Value& _json, ui::text_list& list);
        //void read_shaders_info(const std::string& _path, std::vector<ShadersInfo>& _info);

        void read_transform(const std::string & json_str, math::transform& t);
    }

    namespace json_writer
    {
        std::string serialize(const math::transform& t);
    }
}
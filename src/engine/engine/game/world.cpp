#include "world.h"

#include <cassert>
#include <algorithm>
#include <thread>
#include <new>

#include "engine/core/context.h"
#include "engine/win32/win32_application.h"
#include "engine/core/json_reader.h"
#include "engine/gfx/ogl/ogl_renderer.h"
#include "engine/physics/collision.h"
#include "engine/physics/rigid_body.h"
#include "engine/physics/physics.h"
#include "engine/physics/spring.h"
#include "engine/core/profiler.h"
#include "engine/data/data_system.h"
#include "engine/ui/text_field.h"
#include "engine/ui/text_list.h"
#include "glm.hpp"
#include "engine/network/client.h"


namespace rain::engine
{
    void World::init(const std::string& _path)
    {
        RAIN_PROFILE("world init");

        {
            RAIN_PROFILE("Chunk init");
            vmap = new voxel_map();
            engine::init(vmap);
        }


        file.open(_path);
        json_reader::read_world(file.read(), *this);

        auto chat_view = registry.view<ui::text_field, ui::text_list>();
        for (auto chat : chat_view)
        {
            ui::text_field& field = chat_view.get<ui::text_field>(chat);
            ui::text_list& list = chat_view.get<ui::text_list>(chat);
            field.on_validate.connect_member(RAIN_CONTEXT->app, &application::send_to_network);
            RAIN_MESSAGING->subscribe<network::INGAME_CHAT_INC>(std::bind(&ui::text_list::add_line, &list, std::placeholders::_1));
            break;
        }

        //retrieving main camera
        auto camera_view = registry.view<core::transform, Camera>();
        for (auto entity : camera_view)
        {
            main_camera_id = entity;
            break;
        }

        // ---- temp
        auto entity = registry.create();
        core::transform& t = registry.assign<core::transform>(entity);
        t.scale = glm::vec3{ 0.1f, 0.1f, 0.1f };
        Material& material = registry.assign<Material>(entity);
        material.shader.load(std::string(RAIN_CONFIG->data_root + "/shaders/glsl/model.vs"), std::string(RAIN_CONFIG->data_root + "/shaders/glsl/model.fs"));
        core::mesh& mesh = registry.assign<core::mesh>(entity);

        Model model;
        model.path = file_path(RAIN_CONFIG->data_root + "/models/skelet/skeleton_animated.fbx");
        model.mesh = RAIN_FIND_DATA_FROM_PATH(Mesh, model.path.get_path_absolute());

        u32 vertices_count = model.mesh->data->vertices.size();
        glm::vec3* vertices = (glm::vec3*)malloc(vertices_count * sizeof(glm::vec3));
        for (u32 i = 0; i < vertices_count; ++i)
        {
            memcpy(&vertices[i], &(model.mesh->data->vertices[i].position), sizeof(glm::vec3));
        }

        {
            RAIN_PROFILE("quickhull");
            RAIN_LOG("vertices count : %u", vertices_count);
            quick_hull(vertices, vertices_count, &mesh);
        }
        temp_vao = RAIN_RENDERER->load_primitive(mesh.vertices, mesh.vertices_count, mesh.vertices_indices, mesh.vertices_indices_count, mesh.normals, mesh.normals_count, mesh.normals_indices, mesh.normal_indices_count);
        // ---- /temp
    }

    void World::update_camera(const float _deltaTime)
    {
        auto camera_view = registry.view<core::transform, Camera>();
        for (auto entity : camera_view)
        {
            core::transform& t = camera_view.get<core::transform>(entity);
            Camera& camera = camera_view.get<Camera>(entity);
            update(camera, t);
            RAIN_RENDERER->set_view_matrix(t.position, t.position + camera.front, camera.up);
        }
    }

    void World::update_physics(const float _deltaTime)
    {
        auto chat_view = registry.view<ui::text_field>();
        for (auto chat : chat_view)
        {
            ui::update(chat_view.get(chat));
        }

        // applying springs
        auto spring2_view = registry.view<Spring>();
        for (auto entity : spring2_view)
        {
            Spring& spring2 = spring2_view.get(entity);
            RigidBody& bodyA = registry.get<RigidBody>(spring2.entityA);
            core::transform& transformA = registry.get<core::transform>(spring2.entityA);
            RigidBody& bodyB = registry.get<RigidBody>(spring2.entityB);
            core::transform& transformB = registry.get<core::transform>(spring2.entityB);
            Physics::apply_spring(spring2, transformA, bodyA, transformB, bodyB);
        }

        // updating physics
        auto physics_view = registry.view<RigidBody, core::transform>();
        for (auto entity : physics_view)
        {
            RigidBody& body = physics_view.get<RigidBody>(entity);
            Physics::apply_gravity(body);
            Physics::update(body, physics_view.get<core::transform>(entity), _deltaTime);
        }

        //auto transform_view = registry.view<Transform>();
        //for (auto entity1 : transform_view)
        //{
        //    Transform& transform1 = transform_view.get(entity1);
        //    for (auto entity2 : transform_view)
        //    {
        //        if (entity1 == entity2)
        //            continue;

        //        Transform& transform2 = transform_view.get(entity2);
        //        
        //        detect_collision_gjk(positions, transform1, positions, transform2);
        //    }
        //}
        // updating collision
        auto sphere_view = registry.view<RigidBody, core::sphere, core::transform>();
        for (auto entity1 : sphere_view)
        {
            RigidBody& body1 = sphere_view.get<RigidBody>(entity1);
            core::sphere& bound1 = sphere_view.get<core::sphere>(entity1);
            core::transform& transform1 = sphere_view.get<core::transform>(entity1);
            //for (auto entity2 : sphere_view)
            //{
            //    if (entity1 == entity2)
            //    {
            //        break;
            //    }

            //    RigidBody& body2 = sphere_view.get<RigidBody>(entity2);
            //    Sphere& bound2 = sphere_view.get<Sphere>(entity2);
            //    Transform& transform2 = sphere_view.get<Transform>(entity2);

            //    HitInfo info = detect_collision_sphere(bound1, transform1, bound2, transform2);
            //    if (info.hit)
            //    {
            //        collision_response(body1, transform1, body2, transform2);
            //    }
            //}

            auto plane_view = registry.view<core::plane>();
            for (auto ent_plane : plane_view)
            {
                core::plane& plane = plane_view.get(ent_plane);

                HitInfo info = detect_collision_sphere_plane(bound1, transform1, plane);
                if (info.hit)
                {
                    collision_response(body1, transform1, project_on_plane(transform1.position, plane));
                }
            }

        }

    }

    void World::draw(const float _alpha)
    {
        //RAIN_WPROFILE("world render ", 500.0f, 50.0f, 0.2f, (glm::vec4{ 0.5, 0.8f, 0.2f, 1.0f }));
        RAIN_PROFILE("world render ");
        
        engine::draw(vmap);

        auto view = registry.view<core::transform, Model, Material>();

        for (auto entity : view)
        {
            core::transform& t = view.get<core::transform>(entity);
            Model& model = view.get<Model>(entity);
            Material& material = view.get<Material>(entity);

            //Packet p {};
            //p.senderId = 47;
            //p.sequenceNumber= 12;
            //const char* hello = "qslmdfqkldfh";
            //memcpy(p.data, hello, strlen(hello));
            //
            //SerializedPacket* serialized = (SerializedPacket*)p.Serialize();

            //send_data(RAIN_APPLICATION.client, (char*)serialized, sizeof(SerializedPacket));
            //check_receive_data(RAIN_APPLICATION.client, buffer, sizeof(buffer));
            
            glm::vec3 position = t.position * _alpha + t.lastPosition * (1.0f - _alpha);
            glm::quat orientation = t.orientation * _alpha + t.lastOrientation * (1.0f - _alpha);

            RAIN_RENDERER->draw_mesh(model.mesh->data, material, position, orientation, t.scale);
        }

        auto test_view = registry.view<core::transform, core::mesh, Material>();
        for (auto entity : test_view)
        {
            core::transform& t = test_view.get<core::transform>(entity);
            core::mesh& mesh = test_view.get<core::mesh>(entity);
            Material& material = test_view.get<Material>(entity);

            glm::vec3 position = t.position * _alpha + t.lastPosition * (1.0f - _alpha);
            glm::quat orientation = t.orientation * _alpha + t.lastOrientation * (1.0f - _alpha);

            RAIN_RENDERER->draw_primitive(temp_vao, mesh.vertices_indices_count, material, t.position, t.orientation, t.scale);
        }

        auto view2 = registry.view<core::transform, core::sphere>();

        for (auto entity : view2)
        {
            core::transform& t = view2.get<core::transform>(entity);
            
            glm::vec3 position = t.position * _alpha + t.lastPosition * (1.0f - _alpha);
            glm::quat orientation = t.orientation * _alpha + t.lastOrientation * (1.0f - _alpha);

            RAIN_RENDERER->draw_sphere(position, orientation, t.scale);
        }

        auto chat_view = registry.view<ui::text_field, ui::text_list>();
        for (auto chat : chat_view)
        {
            ui::draw(chat_view.get<ui::text_field>(chat));
            ui::draw(chat_view.get<ui::text_list>(chat));
        }
        //auto plane_view = registry.view<Plane>();
        //for (auto ent_plane : plane_view)
        //{
        //    Plane& plane = plane_view.get(ent_plane);
        //    RAIN_RENDERER->draw_quad(plane, project_on_plane(vec3{0, 15, 0}, plane), vec30.7f, 0.7f, 0));
        //}
    }

    Camera& World::main_camera()
    {
        return registry.get<Camera>(main_camera_id);
    }

    core::transform& World::main_camera_transform()
    {
        return registry.get<core::transform>(main_camera_id);
    }

}
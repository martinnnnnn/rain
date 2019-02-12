#include "world.h"

#include <cassert>

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

#include "engine/network/client.h"


namespace rain::engine
{
    using namespace rain::math;

    void World::init(const std::string& _path)
    {
        RAIN_PROFILE("world init");

        file.open(_path);
        json_reader::read_world(file.read(), *this);

        // ---- temp
        auto entity = registry.create();
        transform& t = registry.assign<transform>(entity);
        t.scale = vec3{ 0.1f, 0.1f, 0.1f };
        Material& material = registry.assign<Material>(entity);
        material.shader.load(std::string(RAIN_CONFIG->data_root + "/shaders/glsl/model.vs"), std::string(RAIN_CONFIG->data_root + "/shaders/glsl/model.fs"));
        math::mesh& mesh = registry.assign<math::mesh>(entity);

        Model model;
        model.path = file_path(RAIN_CONFIG->data_root + "/models/skelet/skeleton_animated.fbx");
        model.mesh = RAIN_FIND_DATA_FROM_PATH(Mesh, model.path.get_path_absolute());

        u32 vertices_count = model.mesh->data->vertices.size();
        vec3* vertices = (vec3*)malloc(vertices_count * sizeof(vec3));
        for (u32 i = 0; i < vertices_count; ++i)
        {
            memcpy(&vertices[i], &(model.mesh->data->vertices[i].position), sizeof(vec3));
        }

        quick_hull(vertices, vertices_count, &mesh);
        temp_vao = RAIN_RENDERER->load_primitive(mesh.vertices, mesh.vertices_count, mesh.vertices_indices, mesh.vertices_indices_count, mesh.normals, mesh.normals_count, mesh.normals_indices, mesh.normal_indices_count);
        // ---- /temp
    }

    void World::update_camera(const float _deltaTime)
    {
        auto camera_view = registry.view<transform, Camera>();
        for (auto entity : camera_view)
        {
            transform& t = camera_view.get<transform>(entity);
            Camera& camera = camera_view.get<Camera>(entity);
            update(camera, t);
            RAIN_RENDERER->set_view_matrix(t.position, t.position + camera.front, camera.up);
        }
    }

    void World::update_physics(const float _deltaTime)
    {
        // applying springs
        auto spring2_view = registry.view<Spring>();
        for (auto entity : spring2_view)
        {
            Spring& spring2 = spring2_view.get(entity);
            RigidBody& bodyA = registry.get<RigidBody>(spring2.entityA);
            transform& transformA = registry.get<transform>(spring2.entityA);
            RigidBody& bodyB = registry.get<RigidBody>(spring2.entityB);
            transform& transformB = registry.get<transform>(spring2.entityB);
            Physics::apply_spring(spring2, transformA, bodyA, transformB, bodyB);
        }

        // updating physics
        auto physics_view = registry.view<RigidBody, transform>();
        for (auto entity : physics_view)
        {
            RigidBody& body = physics_view.get<RigidBody>(entity);
            Physics::apply_gravity(body);
            Physics::update(body, physics_view.get<transform>(entity), _deltaTime);
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
        auto sphere_view = registry.view<RigidBody, sphere, transform>();
        for (auto entity1 : sphere_view)
        {
            RigidBody& body1 = sphere_view.get<RigidBody>(entity1);
            sphere& bound1 = sphere_view.get<sphere>(entity1);
            transform& transform1 = sphere_view.get<transform>(entity1);
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

            auto plane_view = registry.view<plane>();
            for (auto ent_plane : plane_view)
            {
                plane& plane = plane_view.get(ent_plane);

                HitInfo info = detect_collision_sphere_plane(bound1, transform1, plane);
                if (info.hit)
                {
                    collision_response(body1, transform1, project_on_plane(transform1.position, plane));
                }
            }

        }
    }

    void World::render(const float _alpha)
    {
        RAIN_WPROFILE("world render ", 25.0f, 25.0f, 0.2f, (math::vec3{ 0.5, 0.8f, 0.2f }));

        auto view = registry.view<transform, Model, Material>();

        for (auto entity : view)
        {
            transform& t = view.get<transform>(entity);
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
            
            vec3 position = t.position * _alpha + t.lastPosition * (1.0f - _alpha);
            quat orientation = t.orientation * _alpha + t.lastOrientation * (1.0f - _alpha);

            RAIN_RENDERER->draw_mesh(model.mesh->data, material, position, orientation, t.scale);
        }

        auto test_view = registry.view<transform, math::mesh, Material>();
        for (auto entity : test_view)
        {
            transform& t = test_view.get<transform>(entity);
            math::mesh& mesh = test_view.get<math::mesh>(entity);
            Material& material = test_view.get<Material>(entity);

            vec3 position = t.position * _alpha + t.lastPosition * (1.0f - _alpha);
            quat orientation = t.orientation * _alpha + t.lastOrientation * (1.0f - _alpha);

            RAIN_RENDERER->draw_primitive(temp_vao, mesh.vertices_indices_count, material, t.position, t.orientation, t.scale);
        }

        auto view2 = registry.view<transform, sphere>();

        for (auto entity : view2)
        {
            transform& t = view2.get<transform>(entity);
            
            vec3 position = t.position * _alpha + t.lastPosition * (1.0f - _alpha);
            quat orientation = t.orientation * _alpha + t.lastOrientation * (1.0f - _alpha);

            RAIN_RENDERER->draw_sphere(position, orientation, t.scale);
        }

        //auto plane_view = registry.view<Plane>();
        //for (auto ent_plane : plane_view)
        //{
        //    Plane& plane = plane_view.get(ent_plane);
        //    RAIN_RENDERER->draw_quad(plane, project_on_plane(vec3{0, 15, 0}, plane), vec30.7f, 0.7f, 0));
        //}
    }

}
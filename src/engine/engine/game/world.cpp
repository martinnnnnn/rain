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
    void World::init(const std::string& _path)
    {
        RAIN_PROFILE("init");

        file.open(_path);
        JsonReader::read_world(file.read(), *this);
    }

    void World::update_physics(const float _deltaTime)
    {
        // applying springs
        auto spring2_view = registry.view<Spring>();
        for (auto entity : spring2_view)
        {
            Spring& spring2 = spring2_view.get(entity);
            RigidBody& bodyA = registry.get<RigidBody>(spring2.entityA);
            Transform& transformA = registry.get<Transform>(spring2.entityA);
            RigidBody& bodyB = registry.get<RigidBody>(spring2.entityB);
            Transform& transformB = registry.get<Transform>(spring2.entityB);
            Physics::apply_spring(spring2, transformA, bodyA, transformB, bodyB);
        }

        // updating physics
        auto physics_view = registry.view<RigidBody, Transform>();
        for (auto entity : physics_view)
        {
            RigidBody& body = physics_view.get<RigidBody>(entity);
            Physics::apply_gravity(body);
            Physics::update(body, physics_view.get<Transform>(entity), _deltaTime);
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
        auto sphere_view = registry.view<RigidBody, Sphere, Transform>();
        for (auto entity1 : sphere_view)
        {
            RigidBody& body1 = sphere_view.get<RigidBody>(entity1);
            Sphere& bound1 = sphere_view.get<Sphere>(entity1);
            Transform& transform1 = sphere_view.get<Transform>(entity1);
        //    for (auto entity2 : sphere_view)
        //    {
        //        if (entity1 == entity2)
        //        {
        //            break;
        //        }

        //        RigidBody& body2 = sphere_view.get<RigidBody>(entity2);
        //        Sphere& bound2 = sphere_view.get<Sphere>(entity2);
        //        Transform& transform2 = sphere_view.get<Transform>(entity2);

        //        HitInfo info = detect_collision_sphere(bound1, transform1, bound2, transform2);
        //        if (info.hit)
        //        {
        //            collision_response(body1, transform1, body2, transform2);
        //        }
        //    }

            auto plane_view = registry.view<Plane>();
            for (auto ent_plane : plane_view)
            {
                Plane& plane = plane_view.get(ent_plane);

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
        //ConsoleProfiler profiler("render");

        auto view = registry.view<Transform, Model, Material>();

        for (auto entity : view)
        {
            Transform& transform = view.get<Transform>(entity);
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
            
            glm::vec3 position = transform.position * _alpha + transform.lastPosition * (1.0f - _alpha);
            glm::quat orientation = transform.orientation * _alpha + transform.lastOrientation * (1.0f - _alpha);

            RAIN_RENDERER->draw_mesh(model.mesh, material, position, orientation, transform.scale);
        }

        auto view2 = registry.view<Transform, Sphere>();

        for (auto entity : view2)
        {
            Transform& transform = view2.get<Transform>(entity);
            
            glm::vec3 position = transform.position * _alpha + transform.lastPosition * (1.0f - _alpha);
            glm::quat orientation = transform.orientation * _alpha + transform.lastOrientation * (1.0f - _alpha);

            RAIN_RENDERER->draw_sphere(position, 1.0f, orientation);
        }

        auto plane_view = registry.view<Plane>();
        for (auto ent_plane : plane_view)
        {
            Plane& plane = plane_view.get(ent_plane);
            RAIN_RENDERER->draw_quad(plane, project_on_plane(glm::vec3(0, 15, 0), plane), glm::vec3(0.7f, 0.7f, 0));
        }
    }

}
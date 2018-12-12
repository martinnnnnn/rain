#include "world.h"

#include <cassert>

#include "win32/win32_application.h"
#include "core/json_reader.h"
#include "math/transform.h"
#include "gfx/ogl/ogl_renderer.h"
#include "physics/collision.h"
#include "physics/rigid_body.h"
#include "physics/physics.h"
#include "physics/spring.h"


namespace rain
{
    void World::init(const std::string& _path)
    {
        file.open(_path);
        JsonReader::read_world(file.read(), *this);
        
        Scene scene;
        read_scene_fbx(RAIN_CONFIG->data_root + "/models/skelet/skeleton_animated.fbx", &scene);
        mesh = scene.meshes[0];
        RAIN_RENDERER->load_mesh(&mesh);
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

        // updating collision
        auto sphere_view = registry.view<RigidBody, Sphere, Transform>();
        for (auto entity1 : sphere_view)
        {
            RigidBody& body1 = sphere_view.get<RigidBody>(entity1);
            Sphere& bound1 = sphere_view.get<Sphere>(entity1);
            Transform& transform1 = sphere_view.get<Transform>(entity1);
            for (auto entity2 : sphere_view)
            {
                if (entity1 == entity2)
                {
                    break;
                }

                RigidBody& body2 = sphere_view.get<RigidBody>(entity2);
                Sphere& bound2 = sphere_view.get<Sphere>(entity2);
                Transform& transform2 = sphere_view.get<Transform>(entity2);

                HitInfo info = detect_collision_sphere(bound1, transform1, bound2, transform2);
                if (info.hit)
                {
                    collision_response(body1, transform1, body2, transform2);
                }
            }

            auto plane_view = registry.view<Plane>();
            for (auto ent_plane : plane_view)
            {
                Plane& planeBound = plane_view.get(ent_plane);
                HitInfo info = detect_collision_sphere_plane(bound1, transform1, planeBound);
                if (info.hit)
                {
                    collision_response(body1, transform1, planeBound.project(transform1.position));
                }
            }

        }
    }

    void World::render(const float _alpha)
    {
        auto view = registry.view<Transform>();

        for (auto entity : view)
        {
            Transform& transform = view.get(entity);
            glm::vec3 position = transform.position * _alpha + transform.previousPosition * (1.0f - _alpha);
            glm::quat orientation = transform.orientation * _alpha + transform.previousOrientation * (1.0f - _alpha);

            //RAIN_RENDERER->draw_sphere(position, 1.0f, orientation);
            RAIN_RENDERER->draw_mesh(&mesh, position, orientation, glm::vec3(1.0f, 1.0f, 1.0f));
        }
    }

}
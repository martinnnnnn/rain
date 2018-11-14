#include "world.h"

#include <cassert>

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
    }

    void World::update_physics(const float _deltaTime)
    {
        // applying springs
        auto spring_view = registry.view<Spring>();
        for (auto entity : spring_view)
        {
            Spring& spring = spring_view.get(entity);
            RigidBody& body = registry.get<RigidBody>(spring.entity);
            Transform& transform = registry.get<Transform>(spring.entity);
            Physics::apply_spring(spring, transform, body);
        }

        // applying ropes
        auto rope_view = registry.view<SpringRope>();
        for (auto entity : rope_view)
        {
            SpringRope& rope = rope_view.get(entity);
            RigidBody& bodyA = registry.get<RigidBody>(rope.entityA);
            Transform& transformA = registry.get<Transform>(rope.entityA);
            RigidBody& bodyB = registry.get<RigidBody>(rope.entityB);
            Transform& transformB = registry.get<Transform>(rope.entityB);
            Physics::apply_spring(rope, transformA, bodyA, transformB, bodyB);
        }

        // applying spring 2
        auto spring2_view = registry.view<Spring2>();
        for (auto entity : spring2_view)
        {
            Spring2& spring2 = spring2_view.get(entity);
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
        auto view = registry.view<RigidBody, BoundingSphere, Transform>();
        for (auto entity1 : view)
        {
            RigidBody& body1 = view.get<RigidBody>(entity1);
            BoundingSphere& bound1 = view.get<BoundingSphere>(entity1);
            Transform& transform1 = view.get<Transform>(entity1);
            for (auto entity2 : view)
            {
                if (entity1 == entity2)
                {
                    break;
                }

                RigidBody& body2 = view.get<RigidBody>(entity2);
                BoundingSphere& bound2 = view.get<BoundingSphere>(entity2);
                Transform& transform2 = view.get<Transform>(entity2);

                bool collide = detect_collision(body1, bound1, transform1, body2, bound2, transform2);
                if (collide)
                {
                    collision_response(body1, transform1, body2, transform2);
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

            RAIN_RENDERER->render_cube(position, orientation);
        }
    }


}
#include "physics.h"

#include "math/transform.h"

namespace rain
{
    void Physics::init(entt::DefaultRegistry& _registry)
    {
        auto view = _registry.view<Transform, RigidBody>();
        for (auto entity : view)
        {
            RigidBody& body = view.get<RigidBody>(entity);

            init_body(body, view.get<Transform>(entity).currentPosition);
        }
    }



    void Physics::update(entt::DefaultRegistry& _registry, float _deltaTime)
    {
        auto view = _registry.view<Transform, RigidBody>();
        for (auto entity : view)
        {
            RigidBody& body = view.get<RigidBody>(entity);
            rain::update(body, _deltaTime);

            Transform& transform = view.get<Transform>(entity);
            transform.previousPosition = transform.currentPosition;
            transform.currentPosition = body.position;
        }
    }
}
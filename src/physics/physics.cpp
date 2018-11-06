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
            init_body(body, view.get<Transform>(entity).currentPosition, view.get<Transform>(entity).currentOrientation);
            //init_orientation(body, view.get<Transform>(entity).currentOrientation);
        }
    }



    void Physics::update(entt::DefaultRegistry& _registry, float _deltaTime)
    {
        auto view = _registry.view<Transform, RigidBody>();
        for (auto entity : view)
        {
            RigidBody& body = view.get<RigidBody>(entity);
            rain::update(body, _deltaTime);
            //rain::update_orientation(body, _deltaTime);

            Transform& transform = view.get<Transform>(entity);
            transform.previousPosition = transform.currentPosition;
            transform.currentPosition = body.position;
            transform.previousOrientation = transform.currentOrientation;
            transform.currentOrientation = body.orientation;
        }
    }
}
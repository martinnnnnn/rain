#include "physics.h"

#include "math/transform.h"

namespace rain
{
    void Physics::init(entt::DefaultRegistry& _registry)
    {
        auto view = _registry.view<Transform, RigidBodyPosition, RigidBodyOrientation>();
        for (auto entity : view)
        {
            RigidBodyPosition& body = view.get<RigidBodyPosition>(entity);
            init_body(body, view.get<Transform>(entity).currentPosition);

            RigidBodyOrientation& orientation = view.get<RigidBodyOrientation>(entity);
            init_orientation(orientation, view.get<Transform>(entity).currentOrientation);
        }
    }



    void Physics::update(entt::DefaultRegistry& _registry, float _deltaTime)
    {
        auto view = _registry.view<Transform, RigidBodyPosition, RigidBodyOrientation>();
        for (auto entity : view)
        {
            RigidBodyPosition& body = view.get<RigidBodyPosition>(entity);
            rain::update(body, _deltaTime);

            RigidBodyOrientation& orientation = view.get<RigidBodyOrientation>(entity);
            rain::update_orientation(orientation, _deltaTime);

            Transform& transform = view.get<Transform>(entity);
            transform.previousPosition = transform.currentPosition;
            transform.currentPosition = body.position;
            transform.previousOrientation = transform.currentOrientation;
            transform.currentOrientation = orientation.orientation;
        }
    }
}
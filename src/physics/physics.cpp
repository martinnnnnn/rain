#include "physics.h"

#include <Windows.h>

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
            transform.previousOrientation = transform.currentOrientation;
            transform.currentOrientation = body.orientation;

            char buffer[256];
            sprintf_s(buffer, "(%f, %f, %f) - (%f, %f, %f, %f)\n", transform.currentPosition.x, transform.currentPosition.y, transform.currentPosition.z, transform.currentOrientation.x, transform.currentOrientation.y, transform.currentOrientation.z, transform.currentOrientation.w);
            OutputDebugStringA(buffer);
        }
    }
}
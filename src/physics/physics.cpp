#include "physics.h"


using namespace rain;



void Physics::update(entt::DefaultRegistry _registry, float _deltaTime)
{
    auto view = _registry.view<Transform, RigidBody>();
    for (auto entity : view)
    {
        RigidBody& physics = view.get<RigidBody>(entity);
        Transform& transform = view.get<Transform>(entity);

        // compute new position / rotation


        // handle collision

    }
}
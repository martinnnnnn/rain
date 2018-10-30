#include "physics.h"


namespace rain::Physics
{
    void init(entt::DefaultRegistry _registry)
    {
        auto view = _registry.view<Transform, RigidBody>();
        for (auto entity : view)
        {
            RigidBody& body = view.get<RigidBody>(entity);

            init_body(body);
        }
    }



    void update(entt::DefaultRegistry _registry, float _deltaTime)
    {
        auto view = _registry.view<Transform, RigidBody>();
        for (auto entity : view)
        {
            RigidBody& body = view.get<RigidBody>(entity);

            rain::update(body, _deltaTime);
        }
    }
}
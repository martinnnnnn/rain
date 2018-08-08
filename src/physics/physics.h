#pragma once

#include "ecs/ecs.h"
#include "rain.h"
#include "core/math.h"

namespace rain
{
    struct Scene;

    struct Physics : public Component
    {
        glm::vec3 direction;
        float speed;
    };

    Physics* CreateCPhysics();
    void InitCPhysics(Physics* _physics);

    System* CreatePhysicsSystem(Scene* _scene);
    void UpdatePhysics(System* _physicsSystem, f64 deltaTime);

}
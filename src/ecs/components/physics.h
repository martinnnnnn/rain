#pragma once

#include "ecs/ecs.h"

namespace rain
{
    struct Physics : public Component
    {
        glm::vec3 direction;
        float speed;
    };

    Physics* CreateCPhysics();
    void InitCPhysics(Physics* _physics);

    System* CreatePhysicsSystem(Game* _game)
    {

    }

    void UpdatePhysics()
    {

    }

}
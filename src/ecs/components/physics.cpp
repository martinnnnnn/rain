#include "physics.h"


namespace rain
{
    Physics* CreateCPhysics()
    {
        Physics* physics = (Physics*)malloc(sizeof(Physics));
        physics->componentType = ComponentType::PHYSICS;
        return physics;
    }

    void InitCPhysics(Physics* _physics)
    {
        _physics->componentType = ComponentType::PHYSICS;
    }
}
#pragma once


#include "core/types.h"
#include "ecs/ecs_entity.h"

namespace rain
{
    /*struct System
    {
        u64 requirements;
        u32 size;
        u32 capacity;
        u32 resizeStep;
        Entity** entities;
    };*/

    struct System
    {
        u64 requirements;
        Vector<Entity*> entities;
    };

    System* CreateSystem(u64 _requirements, u32 _capacity = 0, u32 _resizeStep = 10);
    void InitEntity(System* _system, u32 _capacity, u32 _resizeStep);
    void RemoveEntity(System* _system, Entity* _entity);
    void AddEntities(System* _system, Entity** _entities, u32 _size);

}
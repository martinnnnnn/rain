#pragma once


#include "core/types.h"
#include "core/containers/vector.h"
#include "ecs/ecs_component.h"

namespace rain
{
    struct Entity
    {
        u64 id;
        u64 flags;
        Vector<Component*> components;
    };

    Entity** CreateEntities(u32 _count, u64 _flags, u32 _capacity = 10, u32 _sizeStep = 5);
    Entity* CreateEntity(u64 _id, u32 _capacity = 0, u32 _sizeStep = 10);
    void InitEntity(Entity* _entity, u64 _id, u32 _capacity = 10, u32 _sizeStep = 5);
    void AddComponent(Entity* _entity, Component* _component);
    void RemoveComponent(Entity* _entity, ComponentType _type);
    Component* FindComponent(Entity* _entity, ComponentType _type);
    bool FitsRequirements(Entity* _entity, u64 _bitmask);
}




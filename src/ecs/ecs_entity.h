#pragma once


#include "core/types.h"
#include "ecs/ecs_component.h"

namespace rain
{
    // TODO(martin): add stepcount
    struct Entity
    {
        u64 id;
        char* name;
        u64 flags;
        Component** components;
        u32 size;
        u32 capacity;
        u32 sizeStep;
    };

    Entity* CreateEntity(u64 _id, const char* _name, u32 _capacity = 0, u32 _sizeStep = 10);
    void AddComponent(Entity* _entity, Component* _component);
    void RemoveComponent(Entity* _entity, ComponentType _type);
    Component* FindComponent(Entity* _entity, ComponentType _type);
    bool FitsRequirements(Entity* _entity, u64 _bitmask);

}




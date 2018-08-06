#pragma once


#include "core/types.h"
#include "core/vector.h"
#include "ecs/ecs_component.h"

namespace rain
{
    // TODO(martin): add stepcount
    struct Entity
    {
        u64 id;
        char* name;
        u64 flags;
        Vector<Component*> components;
    };

    Entity* CreateEntity(u64 _id, const char* _name, u32 _capacity = 0, u32 _sizeStep = 10);
    void InitEntity(Entity* _entity, u64 _id, const char* _name, u32 _capacity, u32 _sizeStep);
    void AddComponent(Entity* _entity, Component* _component);
    Component* RemoveComponent(Entity* _entity, ComponentType _type);
    Component* FindComponent(Entity* _entity, ComponentType _type);
    bool FitsRequirements(Entity* _entity, u64 _bitmask);

}




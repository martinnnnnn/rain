#pragma once


#include "entity.h"
#include "core/memory.h"

namespace rain
{

    void AllocateEntities(Entity* _entities, const size_t size)
    {
        _entities = (Entity*)malloc(sizeof(Entity) * size);
        bzero(_entities, sizeof(Entity) * size);
    }

    // factories
    // pools
    // entity container
    // entity life cycle handler
    // base system utility functinos
}
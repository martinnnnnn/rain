#pragma once


#include <stdint.h>

#include "entity.h"

namespace rain
{
    void UpdateTransforms(Entity* _entities, const unsigned int _size, float _deltaTime);
    bool FitsRequirements(Entity* _entity, uint64_t bitmask);
}
#include "system_transform.h"

#include <iostream>

#include "core/transform.h"


namespace rain
{
    void UpdateTransforms(Entity* _entities, const unsigned int _size, float _deltaTime)
    {
        uint64_t bitmask = (uint64_t)Component::Type::TRANSFORM | (uint64_t)Component::Type::MOVEMENT;

        Entity* end = _entities + _size;
        Entity* iter = _entities;

        while (iter < end)
        {
            if (!FitsRequirements(iter, bitmask))
            {
                continue;
            }

            Transform* transform = GetComponent<Transform>(iter, (uint64_t)Component::Type::TRANSFORM);
            Movement* movement = GetComponent<Movement>(iter, (uint64_t)Component::Type::MOVEMENT);

            //std::cout << "> old position : (" << transform->position.x << "," << transform->position.y << "," << transform->position.z << ")" << std::endl;
            transform->position += movement->direction * movement->speed * _deltaTime;
            std::cout << _deltaTime << std::endl;
        }
    }

    bool FitsRequirements(Entity* _entity, uint64_t bitmask)
    {
        if (IsAMatch(_entity->flags, bitmask, bitmask))
        {
            return true;
        }
        return false;
    }
}
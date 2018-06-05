#include "entity.h"




namespace rain
{
    bool AddComponent(Entity* _entity, uint32_t _componentFlag, Component* _component)
    {
        if (GetBit(_entity->flags, _componentFlag))
        {
            return false;
        }
        TurnOnBit(_entity->flags, _componentFlag);
        _entity->components.push_back(_component);
        return true;
    }
}
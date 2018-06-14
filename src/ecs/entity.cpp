#include "entity.h"

#include <iostream>


namespace rain
{


    Entity* GetEntity(Entities* _entities, const std::string& _name)
    {
        Entity* entity = nullptr;
        for (size_t i = 0; i < _entities->entities.size(); ++i)
        {
            if (_entities->entities[i]->name == _name)
            {
                entity = _entities->entities[i];
                break;
            }
        }
        return entity;
    }

    Entity* GetEntity(Entities* _entities, const int _id)
    {
        Entity* entity = nullptr;
        for (size_t i = 0; i < _entities->entities.size(); ++i)
        {
            if (_entities->entities[i]->id == _id)
            {
                entity = _entities->entities[i];
                break;
            }
        }
        return entity;
    }
}
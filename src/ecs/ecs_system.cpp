#include "ecs_system.h"




namespace rain
{
    System* CreateSystem(u64 _requirements, u32 _capacity, u32 _resizeStep)
    {
        System* system = (System*)malloc(sizeof(System));
        system->requirements = _requirements;
        system->size = 0;
        system->capacity = _capacity;
        system->resizeStep = _resizeStep;

        if (system->capacity > 0)
        {
            system->entities = (Entity**)calloc(system->capacity, sizeof(Entity*));
        }
        return system;
    }

    void CheckForAddEntity(System* _system, Entity* _entity)
    {
        if (FitsRequirements(_entity, _system->requirements))
        {
            if (_system->size == _system->capacity)
            {
                bool wasEmpty = (_system->capacity == 0);
                _system->capacity += _system->resizeStep;
                if (wasEmpty)
                {
                    _system->entities = (Entity**)calloc(_system->capacity, sizeof(Entity*));
                }
                else
                {
                    Entity** temp = (Entity**)realloc(_system->entities, _system->capacity * sizeof(Entity*));
                    _system->entities = temp;
                }
            }

            _system->entities[_system->size] = _entity;
            _system->size++;
        }
    }

    void RemoveEntity(System* _system, Entity* _entity)
    {
        Entity** end = _system->entities + _system->size;
        Entity** iter = _system->entities;

        while (iter < end)
        {
            if ((*iter)->id == _entity->id)
            {
                free(*iter);
                *iter = *(_system->entities + (_system->size - 1));
                _system->entities[_system->size - 1] = NULL;
                _system->size--;
                return;
            }
            iter++;
        }
    }

    void CheckForAddEntities(System* _system, Entity** _entities, u32 _size)
    {
        u32 resize = _system->size;
        for (u32 i = 0; i < _size; ++i)
        {
            if (FitsRequirements(_entities[i], _system->requirements))
            {
                resize++;
            }
        }

        bool wasEmpty = (_system->capacity == 0);
        while (resize >= _system->capacity)
        {
            _system->capacity += _system->resizeStep;
        }
        if (wasEmpty)
        {
            _system->entities = (Entity**)calloc(_system->capacity, sizeof(Entity*));
        }
        else
        {
            Entity** temp = (Entity**)realloc(_system->entities, _system->capacity * sizeof(Entity*));
            _system->entities = temp;
        }
        for (u32 i = 0; i < _size; ++i)
        {
            if (FitsRequirements(_entities[i], _system->requirements))
            {
                _system->entities[_system->size] = _entities[i];
                _system->size++;
            }
        }
    }
}
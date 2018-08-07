#include "ecs_system.h"




namespace rain
{
    System* CreateSystem(u64 _requirements, u32 _capacity, u32 _resizeStep)
    {
        System* system = (System*)malloc(sizeof(System));
        system->requirements = _requirements;

        InitVector<Entity*>(&system->entities, _capacity, _resizeStep);

        return system;
    }

    void InitEntity(System* _system, u32 _capacity, u32 _resizeStep)
    {
        InitVector<Entity*>(&_system->entities, _capacity, _resizeStep);
    }

    void RemoveEntity(System* _system, Entity* _entity)
    {
        RemoveItem<Entity*>(&_system->entities, &_entity);
    }

    bool CheckEntityName(Entity* _entity, const char* _name)
    {
        return strcmp(_entity->name, _name);
    }

    bool CheckEntityId(Entity* _entity, u32 _id)
    {
        return _entity->id == _id;
    }

    Entity* FindEntity(System* _system, const char* _name)
    {
        auto equals = [](Entity* _ent, const char* _name) { return strcmp(_ent->name, _name); };

        Entity** comp = FindItem<Entity*, const char*>(&_system->entities, _name, CheckEntityName);
        if (comp)
            return *comp;

        return nullptr;
    }

    Entity* FindEntity(System* _system, u32 _entityId)
    {
        auto equals = [](Entity* _ent, u32 _id) { return _ent->id == _id; };

        Entity** comp = FindItem<Entity*, u32>( &_system->entities, _entityId, equals);
        if (comp)
            return *comp;

        return nullptr;
    }


    void AddEntities(System* _system, Entity** _entities, u32 _size)
    {
        /*u32 resize = _system->size;
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
        }*/
    }
}
#include "ecs_system.h"

#include "core/containers/vector.h"


namespace rain
{
    System* CreateSystem(u64 _requirements, u32 _capacity, u32 _resizeStep)
    {
        System* system = (System*)malloc(sizeof(System));
        system->requirements = _requirements;

        InitVector<Entity*>(&system->entities, _capacity, _resizeStep);

        return system;
    }

    void InitSystem(System* _system, u32 _capacity, u32 _resizeStep)
    {
        InitVector<Entity*>(&_system->entities, _capacity, _resizeStep);
    }

    void RemoveEntity(System* _system, Entity* _entity)
    {
        RemoveItem<Entity*>(&_system->entities, &_entity);
    }

    Entity* FindEntity(System* _system, u32 _entityId)
    {
        auto equals = [](Entity* _ent, u32 _id) { return _ent->id == _id; };

        Entity** comp = FindItem<Entity*, u32>( &_system->entities, _entityId, equals);
        if (comp)
            return *comp;

        return nullptr;
    }

    // adds every entity with the right requirements
    void AddEntities(System* _system, Vector<Entity*>* _entities)
    {
        Vector<Entity*> toAdd;
        InitVector<Entity*>(&toAdd, _entities->capacity, 0);

        for (u32 i = 0; i < _entities->size; ++i)
        {
            if (FitsRequirements(_entities->items[i], _system->requirements))
            {
                AddItem<Entity*>(&toAdd, _entities->items[i]);
            }
        }
        AddRangeItems<Entity*>(&_system->entities, toAdd.items, toAdd.size);
    }
}
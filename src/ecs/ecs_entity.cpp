#include "ecs_entity.h"



namespace rain
{
    Entity* CreateEntity(u64 _id, const char* _name, u32 _capacity, u32 _sizeStep)
    {
        Entity* entity = (Entity*)calloc(1, sizeof(Entity));
        entity->id = _id;
        entity->sizeStep = _sizeStep;

        entity->name = (char*)malloc((sizeof(_name) + 1));
        strcpy_s(entity->name, sizeof(entity->name) + 1, _name);
        entity->capacity = _capacity;
        if (entity->capacity > 0)
        {
            entity->components = (Component**)calloc(entity->capacity, sizeof(Component*));
        }

        return entity;
    }

    void AddComponent(Entity* _entity, Component* _component)
    {
        u64 flag = ((u64)1 << (u64)_component->componentType);
        if ((_entity->flags & flag) == 0)
        {
            _entity->flags |= flag;

            if (_entity->size == _entity->capacity)
            {
                bool wasEmpty = _entity->capacity == 0;
                _entity->capacity += _entity->sizeStep;
                if (wasEmpty)
                {
                    _entity->components = (Component**)calloc(_entity->capacity, sizeof(Component*));
                }
                else
                {
                    Component** temp = (Component**)realloc(_entity->components, _entity->capacity * sizeof(Component*));
                    _entity->components = temp;
                }
            }

            _entity->components[_entity->size] = _component;
            _entity->size++;
        }
    }

    void RemoveComponent(Entity* _entity, ComponentType _type)
    {
        u64 flag = ((u64)1 << (u64)_type);
        if ((_entity->flags & flag) != 0)
        {
            Component** end = _entity->components + _entity->size;
            Component** iter = _entity->components;

            while (iter < end)
            {
                if ((*iter)->componentType == _type)
                {
                    free(*iter);
                    *iter = *(_entity->components + (_entity->size - 1));
                    _entity->components[_entity->size - 1] = NULL;
                    _entity->size--;
                    return;
                }
                iter++;
            }

        }
    }

    Component* FindComponent(Entity* _entity, ComponentType _type)
    {
        u64 flag = ((u64)1 << (u32)_type);
        if ((_entity->flags & flag) != 0)
        {
            Component** end = _entity->components + _entity->size;
            Component** iter = _entity->components;

            while (iter < end)
            {
                if ((*iter)->componentType == _type)
                {
                    return *iter;
                }
                iter++;
            }
        }

        return nullptr;
    }

    bool FitsRequirements(Entity* _entity, u64 _bitmask)
    {
        if ((_entity->flags & _bitmask) == _bitmask)
        {
            return true;
        }
        return false;
    }
}
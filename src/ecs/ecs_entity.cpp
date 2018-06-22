#include "ecs_entity.h"



namespace rain
{
    Entity* CreateEntity(u64 _id, const char* _name, u32 _capacity, u32 _sizeStep)
    {
        Entity* entity = (Entity*)calloc(1, sizeof(Entity));
        entity->id = _id;
        entity->name = (char*)malloc((sizeof(_name) + 1));
        strcpy_s(entity->name, sizeof(entity->name) + 1, _name);

        entity->components = CreateVector(_capacity, _sizeStep);

        //entity->sizeStep = _sizeStep;

        //entity->capacity = _capacity;
        //if (entity->capacity > 0)
        //{
        //    entity->components = (Component**)calloc(entity->capacity, sizeof(Component*));
        //}

        return entity;
    }

    void AddComponent(Entity* _entity, Component* _component)
    {
        u64 flag = ((u64)1 << (u64)_component->componentType);
        if ((_entity->flags & flag) == 0)
        {
            _entity->flags |= flag;

            Vector* comps = _entity->components;
            AddItem(_entity->components, (void*)_component);
        }
    }

    Component* RemoveComponent(Entity* _entity, ComponentType _type)
    {
        u64 flag = ((u64)1 << (u64)_type);
        if ((_entity->flags & flag) != 0)
        {
            Vector* components = _entity->components;
            Component** end = (Component**)components->items + components->size;
            Component** iter = (Component**)components->items;
            u32 index = 0;
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
                iter++; index++;
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
#include "ecs_entity.h"



namespace rain
{
    Entity* CreateEntity(u64 _id, const char* _name, u32 _capacity, u32 _sizeStep)
    {
        Entity* entity = (Entity*)calloc(1, sizeof(Entity));
        entity->id = _id;
        entity->name = (char*)malloc((sizeof(_name) + 1));
        strcpy_s(entity->name, sizeof(entity->name) + 1, _name);

        InitVector<Component*>(&entity->components, _capacity, _sizeStep);

        return entity;
    }

    Entity** CreateEntities(u32 _count, u64 _flags, u32 _capacity = 10, u32 _sizeStep = 5)
    {
        Entity** entities = (Entity**)malloc(sizeof(Entity) * _count);
        for (int i = 0; i < _count; ++i)
        {
            entities[i] = (Entity*)calloc(1, sizeof(Entity));
            entities[i]->id = 0;
            entities[i]->name = "default name";
            InitVector<Component*>(&entities[i]->components, _capacity, _sizeStep);
        }
        return entities;
    }

    void InitEntity(Entity* _entity, u64 _id, const char* _name, u32 _capacity, u32 _sizeStep)
    {
        _entity->id = _id;
        _entity->name = (char*)malloc((sizeof(_name) + 1));
        strcpy_s(_entity->name, sizeof(_entity->name) + 1, _name);

        InitVector<Component*>(&_entity->components, _capacity, _sizeStep);
    }

    void AddComponent(Entity* _entity, Component* _component)
    {
        u64 flag = ((u64)1 << (u64)_component->componentType);
        if ((_entity->flags & flag) == 0)
        {
            _entity->flags |= flag;

            //Vector<Component*>* comps = _entity->components;
            AddItem<Component*>(&_entity->components, _component);
        }
    }

    void RemoveComponent(Entity* _entity, ComponentType _type)
    {
        Component* comp = FindComponent(_entity, _type);
        if (comp)
        {
            RemoveItem<Component*>(&_entity->components, &comp);
        }
    }

    bool CheckComp(Component* _comp, ComponentType _type)
    {
        return _comp->componentType == _type;
    }


    Component* FindComponent(Entity* _entity, ComponentType _type)
    {
        u64 flag = ((u64)1 << (u32)_type);
        if ((_entity->flags & flag) != 0)
        {
            Component** comp = FindItem<Component*, ComponentType>(&_entity->components, _type, CheckComp);
            if (comp)
                return *comp;

            //Component** end = _entity->components.items + _entity->components.size;
            //Component** iter = _entity->components.items;

            //while (iter < end)
            //{
            //    if ((*iter)->componentType == _type)
            //    {
            //        return *iter;
            //    }
            //    iter++;
            //}
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
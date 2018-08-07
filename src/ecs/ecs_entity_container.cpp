//#include "ecs_entity_container.h"
//
//
//
//namespace rain
//{
//    EntityContainer* CreateEntityContainer(u32 _capacity, u32 _resizeStep, u32 _size)
//    {
//        EntityContainer* container = (EntityContainer*)malloc(sizeof(EntityContainer));
//        container->size = 0;
//        container->resizeStep = _resizeStep;
//        container->capacity = _capacity;
//
//        if (container->capacity > 0)
//        {
//            container->entities = (Entity**)calloc(container->capacity, sizeof(Entity*));
//        }
//
//        if (_size > 0)
//        {
//            Entity* entities = (Entity*)calloc(_size, sizeof(Entity));
//
//            for (u32 i = 0; i < _size; ++i)
//            {
//                entities[container->size].id = container->size;
//                container->entities[container->size] = &entities[container->size];
//                container->size++;
//            }
//        }
//
//        return container;
//    }
//
//
//    void AddEntity(EntityContainer* _container, Entity* _entity)
//    {
//        if (_container->size == _container->capacity)
//        {
//            bool wasEmpty = (_container->capacity == 0);
//            _container->capacity += _container->resizeStep;
//            if (wasEmpty)
//            {
//                _container->entities = (Entity**)calloc(_container->capacity, sizeof(Entity*));
//            }
//            else
//            {
//                Entity** temp = (Entity**)realloc(_container->entities, _container->capacity * sizeof(Entity*));
//                _container->entities = temp;
//            }
//        }
//
//        _container->entities[_container->size] = _entity;
//        _container->size++;
//    }
//
//
//    void RemoveEntity(EntityContainer* _container, Entity* _ent)
//    {
//        Entity** end = _container->entities + _container->size;
//        Entity** iter = _container->entities;
//
//        while (iter < end)
//        {
//            if ((*iter)->id == _ent->id)
//            {
//                free(*iter);
//                *iter = *(_container->entities + (_container->size - 1));
//                _container->entities[_container->size - 1] = NULL;
//                _container->size--;
//                return;
//            }
//            iter++;
//        }
//    }
//}
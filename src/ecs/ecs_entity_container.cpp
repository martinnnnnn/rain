#include "ecs_entity_container.h"



namespace rain
{
    EntityContainer* CreateEntityContainer(u32 _capacity, u32 _resizeStep, u32 _size)
    {
        EntityContainer* container = (EntityContainer*)malloc(sizeof(EntityContainer));
        container->size = 0;
        container->resizeStep = _resizeStep;
        container->capacity = _capacity;

        if (container->capacity > 0)
        {
            container->entities = (Entity**)calloc(container->capacity, sizeof(Entity*));
        }

        if (_size > 0)
        {
            Entity* entities = (Entity*)calloc(_size, sizeof(Entity));

            for (u32 i = 0; i < _size; ++i)
            {
                entities[container->size].id = container->size;
                container->entities[container->size] = &entities[container->size];
                container->size++;
            }
        }

        return container;
    }


    void AddEntity(EntityContainer* _container, Entity* _entity)
    {
        if (_container->size == _container->capacity)
        {
            bool wasEmpty = (_container->capacity == 0);
            _container->capacity += _container->resizeStep;
            if (wasEmpty)
            {
                _container->entities = (Entity**)calloc(_container->capacity, sizeof(Entity*));
            }
            else
            {
                Entity** temp = (Entity**)realloc(_container->entities, _container->capacity * sizeof(Entity*));
                _container->entities = temp;
            }
        }

        _container->entities[_container->size] = _entity;
        _container->size++;
    }


    void RemoveEntity(EntityContainer* _container, Entity* _ent)
    {
        Entity** end = _container->entities + _container->size;
        Entity** iter = _container->entities;

        while (iter < end)
        {
            if ((*iter)->id == _ent->id)
            {
                free(*iter);
                *iter = *(_container->entities + (_container->size - 1));
                _container->entities[_container->size - 1] = NULL;
                _container->size--;
                return;
            }
            iter++;
        }
    }


    Container* CreateContainer(u32 _capacity, u32 _resizeStep, u32 size)
    {
        Container* container = (Container*)malloc(sizeof(Container));
        container->size = 0;
        container->resizeStep = _resizeStep;
        container->capacity = _capacity;

        if (container->capacity > 0)
        {
            container->elements = (void**)calloc(container->capacity, sizeof(void*));
        }

        return container;
    }

    void AddElement(Container* _container, void* _element)
    {
        if (_container->size == _container->capacity)
        {
            bool wasEmpty = (_container->capacity == 0);
            _container->capacity += _container->resizeStep;
            if (wasEmpty)
            {
                _container->elements = (void**)calloc(_container->capacity, sizeof(void*));
            }
            else
            {
                void** temp = (void**)realloc(_container->elements, _container->capacity * sizeof(void*));
                _container->elements = temp;
            }
        }

        _container->elements[_container->size] = _element;
        _container->size++;
    }

    void RemoveElement(Container* _container, void* _element)
    {
        void** end = _container->elements + _container->size;
        void** iter = _container->elements;

        while (iter < end)
        {
            if ((*iter) == _element)
            {
                free(*iter);
                *iter = *(_container->elements + (_container->size - 1));
                _container->elements[_container->size - 1] = NULL;
                _container->size--;
                return;
            }
            iter++;
        }
    }
}
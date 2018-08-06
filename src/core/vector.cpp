#include "vector.h"


#include "stdlib.h"



namespace rain
{
    //Vector* CreateVector(u32 _capacity, u32 _resizeStep)
    //{
    //    Vector* vector = (Vector*)malloc(sizeof(Vector));
    //    vector->size = 0;
    //    vector->resizeStep = _resizeStep;
    //    vector->capacity = _capacity;

    //    if (vector->capacity > 0)
    //    {
    //        vector->items = (void**)calloc(vector->capacity, sizeof(void*));
    //    }

    //    return vector;
    //}

    //void AddItem(Vector* _vector, void* _item)
    //{
    //    if (_vector->size == _vector->capacity)
    //    {
    //        bool wasEmpty = (_vector->capacity == 0);
    //        _vector->capacity += _vector->resizeStep;
    //        if (wasEmpty)
    //        {
    //            _vector->items = (void**)calloc(_vector->capacity, sizeof(void*));
    //        }
    //        else
    //        {
    //            void** temp = (void**)realloc(_vector->items, _vector->capacity * sizeof(void*));
    //            _vector->items = temp;
    //        }
    //    }

    //    _vector->items[_vector->size] = _item;
    //    _vector->size++;
    //}

    //void RemoveItem(Vector* _vector, void* _item)
    //{
    //    void** end = _vector->items + _vector->size;
    //    void** iter = _vector->items;

    //    while (iter < end)
    //    {
    //        if ((*iter) == _item)
    //        {
    //            *iter = *(_vector->items + (_vector->size - 1));
    //            _vector->items[_vector->size - 1] = NULL;
    //            _vector->size--;
    //            return;
    //        }
    //        iter++;
    //    }
    //}

    //void RemoveItem(Vector* _container, u32 _index)
    //{
    //    if (_index < _container->size)
    //    {
    //        _container->items[_index] = *(_container->items + (_container->size - 1));
    //        _container->items[_container->size - 1] = NULL;
    //        _container->size--;
    //        return;
    //    }
    //}
}
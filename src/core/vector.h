#pragma once

#include "core/types.h"

namespace rain
{
    template<typename T>
    struct Vector
    {
        T* items;
        u32 size;
        u32 capacity;
        u32 resizeStep;
    };

    template<typename T>
    Vector<T>* CreateVector(u32 _capacity, u32 _resizeStep)
    {
        Vector<T>* vector = (Vector<T>*)malloc(sizeof(Vector<T>));
        vector->size = 0;
        vector->resizeStep = _resizeStep;
        vector->capacity = _capacity;

        if (vector->capacity > 0)
        {
            vector->items = (T*)calloc(vector->capacity, sizeof(T));
        }

        return vector;
    }

    template<typename T>
    void InitVector(Vector<T>* _vector, u32 _capacity, u32 _resizeStep)
    {
        _vector->size = 0;
        _vector->resizeStep = _resizeStep;
        _vector->capacity = _capacity;

        if (_vector->capacity > 0)
        {
            _vector->items = (T*)calloc(_vector->capacity, sizeof(T));
        }
    }

    template<typename T>
    void AddItem(Vector<T>* _vector, T _item)
    {
        if (_vector->size == _vector->capacity)
        {
            Resize(_vector, _vector->capacity + 1);

            //bool wasEmpty = (_vector->capacity == 0);
            //_vector->capacity += _vector->resizeStep;
            //if (wasEmpty)
            //{
            //    _vector->items = (T*)calloc(_vector->capacity, sizeof(T));
            //}
            //else
            //{
            //    T* temp = (T*)realloc(_vector->items, _vector->capacity * sizeof(T));
            //    _vector->items = temp;
            //}
        }

        _vector->items[_vector->size] = _item;
        _vector->size++;
    }

    template<typename T>
    void RemoveItem(Vector<T>* _vector, T* _item)
    {
        if (!_item || !_vector)
        {
            return;
        }

        T* end = _vector->items + _vector->size;
        T* iter = _vector->items;

        while (iter < end)
        {
            if (*iter == *_item)
            {
                *iter = *(_vector->items + (_vector->size - 1));
                _vector->items[_vector->size - 1] = NULL;
                _vector->size--;
                return;
            }
            iter++;
        }
    }

    template<typename T>
    void RemoveItem(Vector<T>* _vector, u32 _index)
    {
        if (_index < _vector->size)
        {
            _vector->items[_index] = *(_vector->items + (_vector->size - 1));
            _vector->items[_vector->size - 1] = NULL;
            _vector->size--;
        }
    }

    template<typename T, typename I>
    T* FindItem(Vector<T>* _vector, I _check, bool(*_compareFunction)(T, I))
    {
        T* end = _vector->items + _vector->size;
        T* iter = _vector->items;

        while (iter < end)
        {
            if (_compareFunction(*iter, _check))
            {
                return iter;
            }
            iter++;
        }
        return nullptr;
    }

    template<typename T>
    void Resize(Vector<T>* _vector, u32 _newCapacity)
    {
        bool isEmpty = (_vector->capacity == 0);
        while (_newCapacity >= _vector->capacity)
        {
            _vector->capacity += _vector->resizeStep;
        }
        if (isEmpty)
        {
            _vector->items = (T*)calloc(_vector->capacity, sizeof(T));
        }
        else
        {
            T* temp = (T*)realloc(_vector->items, _vector->capacity * sizeof(T));
            _vector->items = temp;
        }
    }

    template<typename T>
    void AddRangeItems(Vector<T>* _vector, T* _range, u32 _rangeCount)
    {
        if (_vector->capacity - _vector->size < _rangeCount)
        {
            Resize(_vector, _vector->size + _rangeCount);
        }
        
        memcpy(&_vector->items[_vector->size], _range, sizeof(T) * _rangeCount);
        _vector->size += _rangeCount;
    }

//#include "vector.inl"

}
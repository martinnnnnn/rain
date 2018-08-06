


template<typename T>
Vector<T>* CreateVector(u32 _capacity, u32 _resizeStep)
{
    Vector<T>* vector = (Vector<T>*)malloc(sizeof(Vector));
    vector->size = 0;
    vector->resizeStep = _resizeStep;
    vector->capacity = _capacity;

    if (vector->capacity > 0)
    {
        vector->items = (void**)calloc(vector->capacity, sizeof(void*));
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
        bool wasEmpty = (_vector->capacity == 0);
        _vector->capacity += _vector->resizeStep;
        if (wasEmpty)
        {
            _vector->items = (T*)calloc(_vector->capacity, sizeof(T));
        }
        else
        {
            T* temp = (T*)realloc(_vector->items, _vector->capacity * sizeof(T));
            _vector->items = temp;
        }
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
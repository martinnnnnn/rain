

template<typename Entity>
void SparseSet<Entity>::construct(Entity _value)
{
    assert(!contains(_value));

    const entity_type position = entity_trait::get_value(_value);

    if (_value > m_sparse.size())
    {
        m_sparse.resize(_value + 1, -1);
    }

    m_sparse[_value] = m_dense.size();
    m_dense.push_back(_value);
}

template<typename Entity>
bool SparseSet<Entity>::contains(Entity _value)
{
    return (_value < m_sparse.size()) && (m_sparse[_value] != -1);
}

template<typename Entity>
void SparseSet<Entity>::destroy(Entity _value)
{
    assert(contains(_value));
    const Entity back = m_dense.back();
    Entity &candidate = m_sparse[_value];
    m_sparse[back] = candidate;
    m_dense[candidate] = back;
    candidate = -1;
    m_dense.pop_back();
}

template<typename Entity>
bool SparseSet<Entity>::empty()
{
    return m_dense.size();
}

template<typename Entity>
void SparseSet<Entity>::reset()
{
    m_sparse.clear();
    m_dense.clear();
}

template<typename Entity>
Entity SparseSet<Entity>::get(Entity _value)
{
    assert(contains(_value));
    return m_sparse[_value];
}
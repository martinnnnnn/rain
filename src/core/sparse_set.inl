

template<typename Entity>
void SparseSet<Entity>::construct(Entity _value)
{
    assert(!contains(_value));

    const entity_type position = entity_trait::get_value(_value);

    if (position >= m_sparse.size())
    {
        m_sparse.resize(position + 1, -1);
    }

    m_sparse[position] = m_dense.size();
    m_dense.push_back(_value);
}

template<typename Entity>
bool SparseSet<Entity>::contains(Entity _value)
{
    const entity_type position = entity_trait::get_value(_value);

    return (position < m_sparse.size()) && (m_sparse[position] != -1);
}

template<typename Entity>
void SparseSet<Entity>::destroy(Entity _value)
{
    assert(contains(_value));
    const entity_type position = entity_trait::get_value(_value);
    const Entity back = m_dense.back();
    Entity &candidate = m_sparse[position];
    m_sparse[entity_trait::get_value(back)] = candidate;
    m_dense[candidate] = back;
    candidate = -1;
    m_dense.pop_back();
}

template<typename Entity>
bool SparseSet<Entity>::empty()
{
    return !m_dense.size();
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
    const entity_type position = entity_trait::get_value(_value);
    return m_sparse[position];
}
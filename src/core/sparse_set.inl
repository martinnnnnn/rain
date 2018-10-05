

template<typename T>
void SparseSet<T>::construct(T _value)
{
    if (_value > m_sparse.size())
    {
        m_sparse.resize(_value + 1, -1);
    }

    m_sparse[_value] = m_dense.size();
    m_dense.push_back(_value);
}

template<typename T>
bool SparseSet<T>::contains(T _value)
{
    return (_value < m_sparse.size()) && (m_sparse[_value] != -1);
}

template<typename T>
void SparseSet<T>::destroy(T _value)
{
    assert(contains(_value));
    const T back = m_dense.back();
    T &candidate = m_sparse[_value];
    m_sparse[back] = candidate;
    m_dense[candidate] = back;
    candidate = -1;
    m_dense.pop_back();
}

template<typename T>
bool SparseSet<T>::empty()
{
    return m_dense.size();
}

template<typename T>
void SparseSet<T>::reset()
{
    m_sparse.clear();
    m_dense.clear();
}

template<typename T>
T SparseSet<T>::get(T _value)
{
    assert(contains(_value));
    return m_sparse[_value];
}
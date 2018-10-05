#pragma once


#include <vector>
#include <cassert>

template <typename T>
class SparseSet
{
public:
    SparseSet(T _startingMaxValueID = 0, T _startingCapacity = 0);

    void construct(T _value);
    bool contains(T _value);
    void destroy(T _value);
    bool empty();
    void reset();
    T get(T _value);

protected:
    std::vector<T> m_sparse;
    std::vector<T> m_dense;
    T currendIndex;
};


template<typename T>
SparseSet<T>::SparseSet(T _startingMaxValueID, T _startingCapacity)
    : m_sparse(_startingMaxValueID)
{
    m_sparse.reserve(_startingMaxValueID);
    m_dense.reserve(_startingCapacity);

}

#include "sparse_set.inl"
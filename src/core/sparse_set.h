#pragma once


#include <vector>
#include <cassert>

#include "ecs/ecs_entity.h"

template <typename Entity>
class SparseSet
{

public:
    SparseSet(u32 _startingMaxValueID = 0, u32 _startingCapacity = 0);

    void construct(Entity _value);
    bool contains(Entity _value);
    void destroy(Entity _value);
    bool empty();
    void reset();
    Entity get(Entity _value);

protected:
    std::vector<Entity> m_sparse;
    std::vector<Entity> m_dense;
    Entity currendIndex;
};


template<typename Entity>
SparseSet<Entity>::SparseSet(u32 _startingMaxValueID, u32 _startingCapacity)
    : m_sparse(_startingMaxValueID)
{
    m_sparse.reserve(_startingMaxValueID);
    m_dense.reserve(_startingCapacity);
}


template<typename Entity>
void SparseSet<Entity>::construct(Entity _entity)
{
    assert(!contains(_entity));

    if (_entity.value >= m_sparse.size())
    {
        m_sparse.resize(_entity.value + 1, null);
    }

    m_sparse[_entity.value] = m_dense.size();
    m_dense.push_back(_entity);
}

template<typename Entity>
bool SparseSet<Entity>::contains(Entity _entity)
{
    return (_entity.value < m_sparse.size()) && (m_sparse[_entity.value] == _entity);
}

template<typename Entity>
void SparseSet<Entity>::destroy(Entity _entity)
{
    assert(contains(_entity));
    const entity_type position = _entity.value;
    const Entity back = m_dense.back();
    Entity &candidate = m_sparse[position];
    m_sparse[back.position] = candidate;
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
Entity SparseSet<Entity>::get(Entity _entity)
{
    assert(contains(_entity));
    return m_sparse[_entity.value];
}
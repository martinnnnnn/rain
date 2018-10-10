#pragma once


#include <vector>
#include <cassert>
#include <limits>

#include "ecs/ecs_entity.h"

class SparseSet
{
    // invalid value is represented as the maximum value 

public:
    SparseSet(u32 _startingMaxValueID = 0, u32 _startingCapacity = 0)
        : m_sparse(_startingMaxValueID)
    {
        m_sparse.reserve(_startingMaxValueID);
        m_dense.reserve(_startingCapacity);
    }

    void construct(u32 _value)
    {
        assert(!contains(_value));
        assert(_value < NULL_VALUE);

        if (_value >= m_sparse.size())
        {
            m_sparse.resize(_value + 1, NULL_VALUE);
        }

        m_sparse[_value] = m_dense.size();
        m_dense.push_back(_value);
    }

    bool contains(u32 _value)
    {
        return (_value < m_sparse.size() && m_sparse[_value] == _value);
    }

    void destroy(u32 _value)
    {
        assert(contains(_value));
        const u32 back = m_dense.back();
        u32 &candidate = m_sparse[_value];
        m_sparse[back] = candidate;
        m_dense[candidate] = back;
        candidate = NULL_VALUE;
        m_dense.pop_back();
    }

    bool empty()
    {
        return !m_dense.size();
    }

    void reset()
    {
        m_sparse.clear();
        m_dense.clear();
    }

    u32 get(u32 _value)
    {
        assert(contains(_value));
        return m_sparse[_value];
    }

    static constexpr u32 NULL_VALUE = Entity::MAX_VALUE;

protected:
    std::vector<u32> m_sparse;
    std::vector<u32> m_dense;
};
#pragma once


#include <vector>
#include <cassert>

#include "ecs/ecs_entity.h"

template <typename Entity>
class SparseSet
{
    using entity_trait = EntityTrait<Entity>;
    using entity_type = typename entity_trait::entity_type;
    using version_type = typename entity_trait::version_type;

public:
    SparseSet(Entity _startingMaxValueID = 0, Entity _startingCapacity = 0);

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
SparseSet<Entity>::SparseSet(Entity _startingMaxValueID, Entity _startingCapacity)
    : m_sparse(_startingMaxValueID)
{
    m_sparse.reserve(_startingMaxValueID);
    m_dense.reserve(_startingCapacity);

}

#include "sparse_set.inl"
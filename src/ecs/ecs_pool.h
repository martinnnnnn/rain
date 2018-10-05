#pragma once

#include "core/sparse_set.h"
#include "core/types.h"
#include "ecs/ecs_entity.h"


//using EntityID = u32;
//using EntitySet = SparseSet<EntityID>;

template<typename Entity, typename Item>
class Pool : public SparseSet<Entity>
{
    using entity_trait = EntityTrait<Entity>;
    using entity_type = typename entity_trait::entity_type;
    using version_type = typename entity_trait::version_type;

public:

    template <typename... Args>
    Item construct(const entity_type _entity, Args &&... _args)
    {
        SparseSet::construct(_entity);
        m_items.emplace_back(std::forward<Args>(_args)...);
        return m_items.back();
    }

    void destroy(const entity_type entity)
    {
        Item tmp = std::move(m_items.back());
        m_items[SparseSet::get(entity)] = std::move(tmp);
        m_items.pop_back();
        SparseSet::destroy(entity);
    }

    const Item& get(entity_type _entity)
    {
        return m_items[SparseSet::get(_entity)];
    }

private:
    std::vector<Item> m_items;
};

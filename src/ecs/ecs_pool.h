#pragma once

#include "ecs/ecs_sparse_set.h"
#include "core/types.h"
#include "ecs/ecs_entity.h"

template <typename Component>
class Pool : public SparseSet
{
public:
    Pool() = default;

    template<typename... Args>
    Component* construct(const u32 _entity, Args &&... _args)
    {
        SparseSet::construct(_entity);
        m_components.emplace_back(std::forward<Args>(_args)...);
        return &(m_components.back());
    }

    Component* get(const u32 _entity)
    {
        assert(m_entities.contains(_entity) && _entity < m_components.size());
        return &(m_components[_entity]);
    }

    bool contains(const u32 _entity)
    {
        return _entity < m_components.size();
    }

    void destroy(const u32 _entity)
    {
        Component tmp = std::move(m_components.back());
        m_components[SparseSet::get(_entity)] = std::move(tmp);
        m_components.pop_back();
        SparseSet::destroy(_entity);
    }


private:
    //SparseSet m_entities;
    std::vector<Component> m_components;
};






//template<typename Entity, typename Item>
//class Pool : public SparseSet<Entity>
//{
//    using entity_trait = EntityTrait<Entity>;
//    using entity_type = typename entity_trait::entity_type;
//    using version_type = typename entity_trait::version_type;
//
//public:
//
//    template <typename... Args>
//    Item& construct(const entity_type _entity, Args &&... _args)
//    {
//        SparseSet::construct(_entity);
//        m_items.emplace_back(std::forward<Args>(_args)...);
//        return m_items.back();
//    }
//
//    void destroy(const entity_type _entity)
//    {
//        Item tmp = std::move(m_items.back());
//        m_items[SparseSet::get(_entity)] = std::move(tmp);
//        m_items.pop_back();
//        SparseSet::destroy(_entity);
//    }
//
//    const Item& get(entity_type _entity)
//    {
//        return m_items[SparseSet::get(_entity)];
//    }
//
//private:
//    std::vector<Item> m_items;
//};

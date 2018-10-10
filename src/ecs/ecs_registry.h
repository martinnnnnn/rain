#pragma once

#include "core/type_id.h"
#include "ecs/ecs_sparse_set.h"
#include "ecs/ecs_pool.h"
#include "ecs/ecs_entity.h"


class Registry
{
    using component_ids = TypeId<struct TDBComponentTypeIDs>;
    using component_id = u32;

public:
    Registry()
        : m_avaliable(0)
        , m_next(0)
    {

    }

    // creates an entity or recycles one if one is avaliable
    Entity create()
    {
        Entity entity {};
        if (m_avaliable)
        {
            u32 nextPosition = m_next;
            u32 version = m_entities[nextPosition].version;
            m_next = m_entities[m_next].value;
            entity.value = nextPosition;
            entity.version = version;
            m_entities[nextPosition] = entity;
            --m_avaliable;
        }
        else
        {
            entity.value = m_entities.size();
            m_entities.push_back(entity);
            assert(entity.value < Entity::MAX_VALUE);
        }
        return entity;
    }

    void destroy(Entity _entity)
    {
        assert(contains(_entity));

        for (u32 i = m_pools.size(); i != 0; --i)
        {
            auto pool = m_pools[i - 1];

            if (pool && pool->contains(_entity.value))
            {
                pool->destroy(_entity.value);
            }
        }

        assert(orphan(_entity));

        const u32 position = _entity.value;
        const u32 version = ++_entity.version;
        const u32 destroyedPosition = (m_avaliable ? m_next : position);
        const Entity destroyedEntity{ destroyedPosition, version };

        m_entities[position] = destroyedEntity;
        m_next = position;

        ++m_avaliable;
    }

    bool orphan(Entity _entity)
    {
        bool is_orphan = true;
        for (u32 i = 0; i < m_pools.size() && is_orphan; ++i)
        {
            auto pool = m_pools[i];
            if (pool && pool->contains(_entity.value))
            {
                is_orphan = false;
            }
        }
        return is_orphan;
    }

    template<typename Component, typename... Args>
    Component* assign(Entity _entity, Args &&... args)
    {
        assert(contains(_entity));
        create_pool<Component>();
        return pool<Component>()->construct(_entity.value, std::forward<Args>(args)...);
    }

    bool contains(Entity _entity)
    {
        assert(_entity.value < m_entities.size());
        return (m_entities[_entity.value] == _entity);
    }

    template<typename Component>
    void create_pool()
    {
        const component_id id = component_ids::get<Component>();

        if (!(id < m_pools.size()))
        {
            m_pools.resize(id + 1);
        }

        if (!m_pools[id])
        {
            m_pools[id] = new Pool<Component>();
        }
    }

    template<typename Component>
    Pool<Component>* pool()
    {
        return static_cast<Pool<Component>*>(m_pools[component_ids::get<Component>()]);
    }

private:


    std::vector<SparseSet*> m_pools;
    std::vector<Entity> m_entities;
    u32 m_next;
    u32 m_avaliable;
};





//template<typename Entity>
//class Registry
//{
//    using etypetrait = EntityTrait<Entity>;
//    using etype = typename etypetrait::entity_type;
//    using eversion = typename etypetrait::version_type;
//    using component_ids = TypeId<struct TDBComponentTypeIDs>;
//    using component_id = u32;
//
//public:
//    Registry()
//        : m_avaliable(0)
//        , m_next(0)
//    {
//
//    }
//
//    // creates an entity or recycles one if one is avaliable
//    etype create()
//    {
//        etype entity;
//        if (m_avaliable)
//        {
//            // get next, which is the location of the next avaliable entity
//            // if any has been deleted before
//            etype entt = m_next;
//            // retrieve new version (which is increased on destroy
//            eversion version = etypetrait::get_version(m_entities[entt]);
//            // m_entities[next] points to another avaliable entity
//            m_next = m_entities[m_next];
//            // new entity is value | version
//            entity = entt | version;
//            // self explainatory
//            m_entities[entt] = entity;
//            --m_avaliable;
//        }
//        else
//        {
//            entity = m_entities.size();
//            m_entities.push_back(entity);
//            assert(entity <= etypetrait::entity_mask);
//        }
//        return entity;
//    }
//
//    void destroy(etype _entity)
//    {
//        assert(contains(_entity));
//
//        for (u32 i = m_pools.size(); i != 0; --i)
//        {
//            auto pool = m_pools[i - 1];
//
//            if (pool && pool->contains(_entity))
//            {
//                pool->destroy(_entity);
//            }
//        }
//
//        assert(orphan(_entity));
//
//        // get position in array
//        const etype position = etypetrait::get_value(_entity);
//        // increment version
//        const eversion version = etypetrait::get_version(_entity);
//        // if avaliable entity, point to it && 
//        const etype newValue = (m_avaliable ? m_next : ((position + 1) & etypetrait::entity_mask)) | version;
//
//        m_entities[position] = newValue;
//        m_next = position;
//
//        ++m_avaliable;
//    }
//
//    bool orphan(etype _entity)
//    {
//        bool is_orphan = true;
//        for (u32 i = 0; i < m_pools.size() && is_orphan; ++i)
//        {
//            auto pool = m_pools[i];
//            if (pool && pool->contains(_entity))
//            {
//                is_orphan = false;
//            }
//        }
//        return is_orphan;
//    }
//
//    template<typename Component, typename... Args>
//    Component& assign(etype _entity, Args &&... args)
//    {
//        assert(contains(_entity));
//        create_pool<Component>();
//        return pool<Component>()->construct(_entity, std::forward<Args>(args)...);
//    }
//
//    bool contains(etype _entity)
//    {
//        u32 position = etypetrait::get_value(_entity);
//        assert(position < m_entities.size());
//        return (m_entities[position] == _entity);
//    }
//
//private:
//
//    template<typename Component>
//    void create_pool()
//    {
//        const component_id id = component_ids::get<Component>();
//
//        if (!(id < m_pools.size()))
//        {
//            m_pools.resize(id + 1);
//        }
//
//        if (!m_pools[id])
//        {
//            m_pools[id] = new Pool<etype, Component>();
//        }
//    }
//
//    template<typename Component>
//    Pool<etype, Component>* pool()
//    {
//        return static_cast<Pool<etype, Component>*>(m_pools[component_ids::get<Component>()]);
//    }
//
//    std::vector<SparseSet<etype>*> m_pools;
//    std::vector<etype> m_entities;
//    etype m_next;
//    u32 m_avaliable;
//};
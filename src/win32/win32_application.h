#pragma once

#include <Windows.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <cassert>

#include <glm/glm.hpp>

#include "core/singleton.h"
#include "core/types.h"
#include "core/sparse_set.h"
#include "core/type_id.h"
#include "ecs/ecs_pool.h"

class Renderer;








template<typename Entity>
class TBD
{
    using etypetrait = EntityTrait<Entity>;
    using etype = typename etypetrait::entity_type;
    using eversion = typename etypetrait::version_type;
    using component_ids = TypeId<struct TDBComponentTypeIDs>;

public:
    TBD()
        : avaliable(0)
        , next = -1
    {

    }

    // creates an entity or recycles one if one is avaliable
    etype create()
    {
        etype entity;
        if (avaliable)
        {
            // get value at next
            // extract version and add 1 
            // entity =  value | version
            // next 
            --avaliable;
        }
        else
        {
            entity = m_entities.size();
            m_entities.push_back(entity);
            assert(entity <= etypetrait::entity_mask);
        }
        return entity;
    }


    template<typename Component, typename... Args>
    Component& assign(EntityID _entity, Args &&... args)
    {
        assert(contains(_entity));
        create_pool<Component>();
        return pool<Component>().construct(_entity, std::forward<Args>(args)...);
    }

    bool contains() { return true; }

private:

    template<typename Component>
    void create_pool()
    {
        const EntityID component_id = component_ids::get<Component>();

        if (!(component_id < m_pools.size()))
        {
            m_pools.resize(component_id + 1);
        }

        if (!m_pools[component_id])
        {
            m_pools[component_id] = new Pool<Component>();
        }
    }

    template<typename Component>
    Pool<Component>* pool()
    {
        return static_cast<Pool<Component>*>(*m_pools[component_ids::get<Component>()]);
    }

    std::vector<EntitySet*> m_pools;
    std::vector<etype> m_entities;
    u32 next;
    u32 avaliable;
};



struct Name
{
    Name() : Name("default") {}

    Name(const std::string& _name) :
        name(_name)
    {
    }

    std::string name;
};

struct Transform
{
    Transform() : Transform(glm::vec3(0.0, 0.0, 0.0)) {}

    Transform(const glm::vec3& _position) :
        position(_position)
    {

    }

    glm::vec3 position;
};

struct Physics
{
    Physics() : Physics(glm::vec3(0.0, 0.0, 0.0), 0.0) {}

    Physics(const glm::vec3& _direction, float _speed) :
        direction(_direction),
        speed(_speed)
    {
    }

    glm::vec3 direction;
    float speed;
};




class Application
{
public:
    int init(HINSTANCE _hinstance, const std::string& _config);
    void update();
    void render();
private:
    HINSTANCE hinstance;
    Renderer* renderer;
};
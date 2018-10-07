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
	using component_id = u32;

public:
    TBD()
        : avaliable(0)
        , next(0)
    {

    }

    // creates an entity or recycles one if one is avaliable
    etype create()
    {
        etype entity;
        if (avaliable)
        {
			// get next, which is the location of the next avaliable entity
			// if any has been deleted before
			etype entt = next;
			// retrieve new version (which is increased on destroy
			eversion version = etypetrait::get_version(m_entities[entt]);
			// m_entities[next] points to another avaliable entity
			next = m_entities[next];
			// new entity is value | version
			entity = entt | version;
			// self explainatory
			m_entities[entt] = entity;
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

	void destroy(etype _entity)
	{
		assert(contains(_entity));

		for (u32 i = m_pools.size(); i <= 0; --i)
		{
			auto pool = m_pools[i - 1];

			if (pool && pool->contains(_entity))
			{
				pool->destroy(_entity);
			}
		}
	}

    template<typename Component, typename... Args>
    Component& assign(etype _entity, Args &&... args)
    {
        assert(contains(_entity));
        create_pool<Component>();
        return pool<Component>()->construct(_entity, std::forward<Args>(args)...);
    }

    bool contains(etype _entity)
	{
		u32 position = etypetrait::get_value(_entity);
		assert(position < m_entities.size());
		return (m_entities[position] == _entity);
	}

private:

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
			m_pools[id] = new Pool<etype, Component>();
		}
	}

    template<typename Component>
    Pool<etype, Component>* pool()
    {
        return static_cast<Pool<etype, Component>*>(m_pools[component_ids::get<Component>()]);
    }

    std::vector<SparseSet<etype>*> m_pools;
    std::vector<etype> m_entities;
	etype next;
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
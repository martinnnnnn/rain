#pragma once


#include <string>
#include <stdint.h>
#include <vector>

#include "graphics/mesh.h"
#include "core/bitmask.h"
#include "core/transform.h"
#include "ecs/component.h"


namespace rain
{

    struct CModel : Component
    {
        
    };

	struct Entity
	{
		int id;
		char* name;
		uint64_t flags;
		std::vector<Component*> components;
	};

    Entity* CreateEntity(int _id, char* _name = "", uint64_t _flags = 0);

    template<typename T>
    bool AddComponent(Entity* _entity, T* _component);
    template<typename T>
    T* CreateComponent(Entity* _entity);

    // ****
    // /!\ : getcomponent performs a static_cast to type T
    // no runtime check occurs so make sure to give the right flag
    // TODO(martin): change this behavior to be more reliable ?
    // TODO(martin): add support for multiple components of the same type
    // ****
    template<typename T>
    T* GetComponent(Entity* _entity, uint64_t _componentFlag);


    struct EntityContainer
    {
        std::vector<Entity*> entities;
    };


    Entity* GetFreeEntity(EntityContainer* _container);
    void ReleaseEntity(EntityContainer* _container, Entity* _entity);


    Entity* GetEntity(EntityContainer* _container, const std::string& _name);
    Entity* GetEntity(EntityContainer* _container, const int _id);


}

#include "entity.inl"


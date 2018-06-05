#pragma once


#include <string>
#include <stdint.h>

#include "graphics/mesh.h"
#include "core/bitmask.h"
#include "core/transform.h"
#include "core/component.h"
namespace rain
{

    struct CModel : Component
    {
        CModel() : Component(Type::MODEL) {}
    };

	struct Entity
	{
		int id;
		std::string name;
		uint32_t flags;
		std::vector<Component*> components;
	};

    struct Entities
    {
        
    };

    template<typename T>
    bool AddComponent(Entity* _entity, T* _component);
    template<typename T>
    T* CreateComponent(Entity* _entity);

    // ****
    // be careful : getcomponent performs a static_cast to type T
    // no runtime check occurs so make sure to give the right flag
    // TODO(martin): change this behavior to be more reliable ?
    // TODO(martin): add support for multiple components of the same type
    // ****
    template<typename T>
    T* GetComponent(Entity* _entity, uint32_t _componentFlag);

}

#include "entity.inl"


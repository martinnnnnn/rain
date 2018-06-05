#pragma once


#include <string>
#include <stdint.h>

#include "graphics/mesh.h"
#include "core/bitmask.h"
#include "core/transform.h"

namespace rain
{

	struct Component
	{
		enum class Type
		{
			NONE		= (1 << 0),
			TRANSFORM	= (1 << 1),
			MODEL		= (1 << 2),
			MAX_COUNT	= (1 << 31)
		};

		Type type;

		Component(Type _type) : type(_type) {}
		virtual ~Component() {}
	};

	struct CModel : Component
	{
        CModel() : Component(Type::MODEL) {}
		std::vector<Model>* model;
	};

    struct CTransform : Component
    {
        CTransform() : Component(Type::TRANSFORM) {}
        Transform transform;
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
    T* CreateComponent(Entity* _entity);

    // ****
    // be careful : getcomponent performs a static_cast to type T
    // no runtime check occurs
    // make sure to give the right flag
    // TODO(martin): change this behavior to be more reliable
    // ****
    template<typename T>
    T* GetComponent(Entity* _entity, uint32_t _componentFlag);

    bool AddComponent(Entity* _entity, uint32_t _componentFlag, Component* _component);
}

#include "entity.inl"


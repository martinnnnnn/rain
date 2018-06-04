#pragma once


#include <string>
#include <stdint.h>

#include "graphics/mesh.h"


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

		explicit Component(Type _type) : type(_type) {}
		virtual ~Component() {}
	};

	struct CModel
	{
		Model* model;
	};

	struct Entity
	{
		int id;
		std::string name;
		uint32_t flags;
		std::vector<Component*> component;

		
	};
}
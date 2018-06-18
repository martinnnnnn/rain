#pragma once


#include "core/types.h"
#include <glm/glm.hpp>

namespace rain
{
   
    
    //------------------------------------------
    
    struct Component
    {
    	// entity uses a bitmask to register which component it contains
    	// this enum doesnt represent bitflags
    	// it is used to shift bits the right number of times to make a bitmask -- i think that should work
    	enum class Type : u32
    	{
    		TRANSFORM = 0,
    		SCRIPT,
    		PHYSICS,
    		MODEL,
    		MAX_COUNT
    	};
    
    	Component::Type type;
    };

    
    
    //------------------------------------
    //void SystemPhysicsUpdate(System* _system)
    //{
    //	for (u32 i = 0; i < _system->size; ++i)
    //	{
    //		Physics* physics = (Physics*)FindComponent(_system->entities[i], Component::Type::PHYSICS);
    //		TransformS* transform = (TransformS*)FindComponent(_system->entities[i], Component::Type::TRANSFORM);
    //
    //		transform->position = transform->position + ((physics->direction * physics->speed) + (physics->friction));
    //
    //		PrintTransform(transform);
    //	}
    //}
}
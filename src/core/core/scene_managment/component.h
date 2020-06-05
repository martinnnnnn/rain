#pragma once


#include "core/rtti/rtti_object.h"

namespace rain::core
{
    struct Component : public RttiObject
    {
        RAIN_DECLARE_RTTI
        
        virtual ~Component() {}

        virtual void            init() {};
        virtual void            shutdown() {};
        virtual void            tick() {};

        void                    add(Component* component);

        template<typename T> 
        T*                      get_component();

        Actor* actor;
    };

    template<typename T>
    T* Component::get_component()
    {
        return actor->get_component<T>();
    }
}
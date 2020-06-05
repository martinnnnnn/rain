#include "component.h"

namespace rain::core
{
    RAIN_IMPLEMENT_RTTI(rain::core, Component, RttiObject);


    void Component::add(Component * component)
    {
        actor->add(component);
    }
}


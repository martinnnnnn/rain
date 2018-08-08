#pragma once

#include "ecs/ecs_component.h"

namespace rain
{
    struct CName : public Component
    {
        char* name;
    };

    CName* CreateName(const char* _name);
    void InitName(CName* _nameComponent, const char* _name);
}
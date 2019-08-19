#pragma once

#include "core/core.h"

#include <vector>

namespace rain::engine
{
    typedef u32 actor_id;

    struct actor
    {
        actor* parent;
        std::vector<actor*> children;
        core::heterogenous_vector<struct COMPONENT_TYPE_ID> components;

    };
}


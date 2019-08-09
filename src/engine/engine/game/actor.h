#pragma once

#include "core/core.h"

namespace rain::engine
{
    typedef u32 actor_id;

    struct actor
    {
        core::heterogenous_vector<struct COMPONENT_TYPE_ID> components;
    };
}


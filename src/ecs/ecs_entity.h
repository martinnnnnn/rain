#pragma once

#include "core/types.h"

#include <limits>





struct Entity
{
    Entity(u32 _value, u32 _version)
        : value(_value)
        , version(_version)
    {}

    Entity() : value(0), version(0) {}

    bool operator==(Entity _entity) const
    {
        return _entity.value == value && _entity.version == version;
    }

    u32 value;
    u32 version;

    static constexpr u32 MAX_VALUE = 0xFFFFFFFF;
    static constexpr u32 MAX_VERSION = 0xFFFFFFF;
};
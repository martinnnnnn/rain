#pragma once

#include <string>

#include "core/types.h"

namespace rain
{
    struct var_info
    {
        var_info(const std::string & _name, u32 _size = 0)
            : name(_name)
            , size(_size)
        {}

        virtual ~var_info() {}

        std::string name;
        u32 size;

    };
}
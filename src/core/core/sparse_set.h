#pragma once


#include <vector>

#include "core/types.h"

namespace rain::core
{
    constexpr u32 default_max_id_value = 512;
    constexpr u32 default_max_id_count = 512;

    struct sparse_set
    {
        sparse_set()
            
        {
            sparse.reserve(default_max_id_value);
            dense.reserve(default_max_id_count);
        }

        u32 create()
        {

        }

    private:
        std::vector<u32> sparse;
        std::vector<u32> dense;
    };
}
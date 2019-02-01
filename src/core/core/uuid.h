#pragma once


#include <string>
#include "core/types.h"

namespace rain::core
{
    class uuid
    {
    public:
        u8 data[16];

        static uuid nil();
        static uuid random();
        static uuid from_string(const std::string& str);
        static uuid from_name(const std::string& name);

        static std::string to_str(const uuid& id);
    };

    inline bool operator==(const uuid& a, const uuid& b)
    {
        return a.data == b.data;
    }
}

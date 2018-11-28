#pragma once

#include <string>

namespace rain
{
    struct var_info
    {
        enum class Type
        {
            UNKNOWN,
            NATIVE,
            OBJECT,
            ARRAY
        };

        var_info(const std::string & _name, Type _type = Type::UNKNOWN) :
            name(_name)
            , type(_type)
        {}

        virtual ~var_info() {}

        std::string name;
        Type type;
    };
}
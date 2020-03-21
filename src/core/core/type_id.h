#pragma once

#include "core/core.h"

#include <atomic>

namespace rain::core
{
    template<typename...>
    class type_id
    {
    public:
        //static const u32 MAX_VALUE = std::numeric_limits<u32>::max();

        template<typename... Type>
        static u32 get()
        {
            return get_id<std::decay_t<Type>...>();
        }
    private:
        static u32 current_id;

        template<typename...>
        static u32 get_id()
        {
            static const u32 value = current_id++;
            return value;
        }
    };

    template<typename... Types>
    u32 type_id<Types...>::current_id{};
}
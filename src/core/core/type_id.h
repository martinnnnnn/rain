#pragma once

#include "core/core.h"


namespace rain::core
{
    template<typename...>
    class TypeId
    {
    public:
        template<typename... Type>
        static u32 get()
        {
            return get_id<std::decay_t<Type>...>();
        }
    private:
        static u32 m_currentId;

        template<typename...>
        static u32 get_id()
        {
            static const u32 value = m_currentId++;
            return value;
        }
    };

    template<typename... Types>
    u32 TypeId<Types...>::m_currentId{};
}
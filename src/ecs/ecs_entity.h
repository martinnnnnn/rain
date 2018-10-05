#pragma once

#include "core/types.h"



template<typename>
struct EntityTrait;


/*
* entity traits for 16 identifier
* entity  : 12 bits [0 -> 4095]
* version : 4 bits  [0 -> 15]
*/
template<>
struct EntityTrait<u16>
{
    using entity_type = u16;
    using version_type = u8;

    static constexpr u16 entity_mask = 0xFFF;
    static constexpr u16 version_mask = 0xF;
    static constexpr auto entity_shift = 12;

    static constexpr entity_type get_value(const entity_type _entity)
    {
        return _entity & entity_mask;
    }

    static constexpr version_type get_version(const entity_type _entity)
    {
        return _entity & (version_mask << entity_shift);
    }
};

/*
* entity traits for 32 identifier
* entity  : 20 bits [0 -> 1 048 575]
* version : 12 bits [0 -> 4095]
*/
template<>
struct EntityTrait<u32>
{
    using entity_type = u32;
    using version_type = u16;

    static constexpr u32 entity_mask = 0xFFFFF;
    static constexpr u32 version_mask = 0xFFF;
    static constexpr auto entity_shift = 20;

    static constexpr entity_type get_value(const entity_type _entity)
    {
        return _entity & entity_mask;
    }

    static constexpr version_type get_version(const entity_type _entity)
    {
        return _entity & (version_mask << entity_shift);
    }
};

/*
* entity traits for 64 identifier
* entity  : 32 bits [0 -> alot]
* version : 32 bits [0 -> alot]
*/
template<>
struct EntityTrait<u64>
{
    using entity_type = u64;
    using version_type = u32;

    static constexpr u64 entity_mask = 0xFFFFFFFF;
    static constexpr u64 version_mask = 0xFFFFFFFF;
    static constexpr auto entity_shift = 20;

    static constexpr entity_type get_value(const entity_type _entity)
    {
        return _entity & entity_mask;
    }

    static constexpr version_type get_version(const entity_type _entity)
    {
        return _entity & (version_mask << entity_shift);
    }
};
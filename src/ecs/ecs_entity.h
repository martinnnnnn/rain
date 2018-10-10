#pragma once

#include "core/types.h"

#include <limits>


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




struct Null
{


    explicit constexpr Null() = default;

    template <typename Entity>
    constexpr operator Entity() const
    {
        Entity entity;
        entity.value = Entity::MAX_VALUE;
        entity.version = Entity::MAX_VERSION;
        return entity;
    }

    constexpr bool operator==(Null) const
    {
        return true;
    }

    constexpr bool operator!=(Null) const
    {
        return false;
    }

    template<typename Entity>
    constexpr bool operator==(const Entity entity) const
    {
        return entity == static_cast<Entity>(*this);
    }

    template<typename Entity>
    constexpr bool operator!=(const Entity entity) const
    {
        return entity != static_cast<Entity>(*this);
    }
};


template<typename Entity>
constexpr bool operator==(const Entity entity, Null null)
{
    return null == entity;
}


template<typename Entity>
constexpr bool operator!=(const Entity entity, Null null)
{
    return null != entity;
}

constexpr Null null = Null{};
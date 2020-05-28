#pragma once

#include <stdint.h>
#include <glm.hpp>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef u32 uptr;

typedef glm::ivec3 ivec3;
typedef glm::uvec3 uvec3;

struct ivec3_hasher
{
    inline std::size_t operator()(const ivec3& position) const;

    inline bool operator()(const ivec3& position_a, const ivec3& position_b) const;
};



std::size_t ivec3_hasher::operator()(const ivec3& position) const
{
    std::size_t seed = 3;

    seed ^= position.x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= position.y + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= position.z + 0x9e3779b9 + (seed << 6) + (seed >> 2);

    return seed;
}

bool ivec3_hasher::operator()(const ivec3& position_a, const ivec3& position_b) const
{
    return position_a.x == position_b.x
        && position_a.y == position_b.y
        && position_a.z == position_b.z;
}


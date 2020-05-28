#pragma once

#include "glm.hpp"
#include <vector>
#include "float.h"

#include "core/core.h"
#include "engine/voxel2/volume_sample.h"
#include "engine/voxel2/volume_size.h"

namespace rain::engine::voxel2
{
    struct VolumeChunk
    {
        static constexpr i32 DEFAULT_SIZE = 32;

        inline VolumeChunk(ivec3 i, i32 s = DEFAULT_SIZE);

		inline i32              offset_from_global(i32 x, i32 y, i32 z) const;

        inline const Sample&    get(u32 offset) const;
        inline const Sample&    get_from_local(u32 x, u32 y, u32 z) const;
        inline const Sample&    get_from_local(const uvec3& position) const;

        inline const Sample&    get_from_global(i32 x, i32 y, i32 z) const;
        inline const Sample&    get_from_global(const uvec3& position) const;

        inline void             set(const Sample& sample, u32 offset);
        inline void             set(const Sample& sample, u32 x, u32 y, u32 z);
        inline void             set(const Sample& sample, uvec3 position);

        ivec3                   index;
        VolumeSize              size;
        std::vector<Sample>     samples;
    };


    VolumeChunk::VolumeChunk(ivec3 i, i32 s)
        : index(i)
        , size(s)
    {
        samples.resize(size.cubed);
    }

    i32 VolumeChunk::offset_from_global(i32 x, i32 y, i32 z) const
    {
        const i32 xoff = (x - index.x * size.value);
        const i32 yoff = (y - index.y * size.value) * size.value;
        const i32 zoff = (z - index.z * size.value) * size.squared;

        return xoff + yoff + zoff;
    }

    const Sample& VolumeChunk::get(u32 offset) const 
    {
        return samples[offset];
    }

    const Sample& VolumeChunk::get_from_local(u32 x, u32 y, u32 z) const 
    { 
        return get(x + y * size.value + z * size.squared); 
    }

    const Sample& VolumeChunk::get_from_local(const uvec3& position) const 
    { 
        return get_from_local(position.x, position.y, position.z);
    }

    const Sample& VolumeChunk::get_from_global(i32 x, i32 y, i32 z) const
    { 
        return get(offset_from_global(x, y, z)); 
    }

    const Sample& VolumeChunk::get_from_global(const uvec3& position) const 
    {
        return get(offset_from_global(position.x, position.y, position.z));
    }

    void VolumeChunk::set(const Sample& sample, u32 offset)
    { 
        samples[offset] = sample; 
    }

    void VolumeChunk::set(const Sample& sample, u32 x, u32 y, u32 z) 
    {
        set(sample, x + y * size.value + z * size.squared);
    }

    void VolumeChunk::set(const Sample& sample, uvec3 position)
    {
        set(sample, position.x, position.y, position.z); 
    }
}



#pragma once

#include "glm.hpp"
#include <vector>
#include "float.h"
#include <future>
#include <string>

#include "core/core.h"
#include "engine/voxel2/volume_sample.h"
#include "engine/voxel2/volume_size.h"

namespace rain::engine::voxel2
{
    struct VolumeChunk
    {
        static constexpr i32 DEFAULT_SIZE = 32;

        inline VolumeChunk(ivec3 i, i32 s = DEFAULT_SIZE);

        void                    init_values_simplex();
        void                    init_values_simplex_async();
        inline bool             is_initialized_async();

        inline const Sample&    get_local(u32 offset) const;
        inline const Sample&    get_local(u32 x, u32 y, u32 z) const;
        inline const Sample&    get_local(const uvec3& position) const;

        inline const Sample&    get_global(i32 offset) const;
        inline const Sample&    get_global(i32 x, i32 y, i32 z) const;
        inline const Sample&    get_global(const uvec3& position) const;

        inline void             set_local(const Sample& sample, u32 offset);
        inline void             set_local(const Sample& sample, u32 x, u32 y, u32 z);
        inline void             set_local(const Sample& sample, uvec3 position);

        inline void             set_global(const Sample& sample, i32 offset);
        inline void             set_global(const Sample& sample, i32 x, i32 y, i32 z);
        inline void             set_global(const Sample& sample, const ivec3& position);


        ivec3                   index;
        VolumeSize              size;
        std::vector<Sample>     samples;

    private:

		inline i32              offset_from_global(i32 x, i32 y, i32 z) const;

        std::future<void> init_future;
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

        const i32 offset = xoff + yoff + zoff;
        assert(offset >= 0);

        return offset;
    }

    const Sample& VolumeChunk::get_local(u32 offset) const
    {
        return samples[offset];
    }

    const Sample& VolumeChunk::get_local(u32 x, u32 y, u32 z) const 
    { 
        return get_local(x + y * size.value + z * size.squared);
    }

    const Sample& VolumeChunk::get_local(const uvec3& position) const 
    { 
        return get_local(position.x, position.y, position.z);
    }

    const Sample& VolumeChunk::get_global(i32 offset) const
    {
        return get_local(offset);
    }

    const Sample& VolumeChunk::get_global(i32 x, i32 y, i32 z) const
    { 
        return get_local(offset_from_global(x, y, z));
    }

    const Sample& VolumeChunk::get_global(const uvec3& position) const 
    {
        return get_local(offset_from_global(position.x, position.y, position.z));
    }

    void VolumeChunk::set_local(const Sample& sample, u32 offset)
    { 
        samples[offset] = sample; 
    }

    void VolumeChunk::set_local(const Sample& sample, u32 x, u32 y, u32 z)
    {
        set_local(sample, x + y * size.value + z * size.squared);
    }

    void VolumeChunk::set_local(const Sample& sample, uvec3 position)
    {
        set_local(sample, position.x, position.y, position.z);
    }

    void VolumeChunk::set_global(const Sample& sample, i32 offset)
    {
        assert(false && "NOT IMPLEMENTED");
    }

    void VolumeChunk::set_global(const Sample& sample, i32 x, i32 y, i32 z)
    {
        set_local(sample, offset_from_global(x, y, z));
    }

    void VolumeChunk::set_global(const Sample& sample, const ivec3& position)
    {
        set_local(sample, offset_from_global(position.x, position.y, position.z));
    }

    bool VolumeChunk::is_initialized_async()
    {
        return init_future.valid();
    }
}



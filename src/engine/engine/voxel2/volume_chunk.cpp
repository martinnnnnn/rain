#include "volume_chunk.h"

namespace rain::engine::voxel2
{

    void VolumeChunk::init_values_simplex()
    {
        for (i32 i = 0; i < size.value; ++i)
        {
            for (i32 j = 0; j < size.value; ++j)
            {
                for (i32 k = 0; k < size.value; ++k)
                {
                    const ivec3 sample_map_pos = (index * size.value) + ivec3{ i, j, k };

                    const f32 distf = core::simplex_noise::noise(
                        f32(sample_map_pos.x) / f32(size.value),
                        f32(sample_map_pos.y) / f32(size.value),
                        f32(sample_map_pos.z) / f32(size.value)) * 127.0f;

                    set_local(Sample(i8(distf)), i, j, k);
                }
            }
        }
    }

    void VolumeChunk::init_values_simplex_async()
    {
        init_future = std::async([this]()
        {
            init_values_simplex();
        });
    }
}
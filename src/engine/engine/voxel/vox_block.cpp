#include "vox_block.h"

#include "vox_map.h"
#include "engine/core/context.h"

namespace rain::engine::voxel 
{
    vox_block::vox_block()
        : map(nullptr)
        , position(0, 0, 0)
        , LOD(LOD_0)
        , need_update(true)
        , vao(0)
        , ebo(0)
        , vbo(0)
    {
        std::fill_n(children, CHILD_COUNT, nullptr);
    }

    void init_simplex(vox_block* block, float frequency, float amplitude, float lacunarity, float persistence)
    {
        block->need_update = true;

        for (u32 i = 0; i < BLOCK_SIZE; ++i)
        {
            for (u32 j = 0; j < BLOCK_SIZE; ++j)
            {
                for (u32 k = 0; k < BLOCK_SIZE; ++k)
                {
                    vox_sample* sample = &(block->samples[i + j * BLOCK_SIZE + k * BLOCK_SIZE_SQUARED]);

                    sample->x = i;
                    sample->y = j;
                    sample->z = k;

                    const vox_position world_position = vox_position
                    { 
                        block->map->position.x + (block->position.x) * BLOCK_SIZE + i32(i),
                        block->map->position.y + (block->position.y) * BLOCK_SIZE + i32(j),
                        block->map->position.z + (block->position.z) * BLOCK_SIZE + i32(k)
                    };

                    const f32 samplex = f32(world_position.x) / f32(BLOCK_SIZE);
                    const f32 sampley = f32(world_position.y) / f32(BLOCK_SIZE);
                    const f32 samplez = f32(world_position.z) / f32(BLOCK_SIZE);

                    core::simplex_noise n(frequency, amplitude, lacunarity, persistence);
                    sample->dist = i8(-world_position.y + (n.fractal(3, samplex, samplez)) * 50.0f);
                    //sample->dist = i8(core::simplex_noise::noise(samplex, sampley, samplez) * 127.0f);

                    if (world_position.y == block->map->min_y || world_position.y == block->map->position.y + block->map->max_y * BLOCK_SIZE - 1)
                    {
                        sample->dist = -10;
                    }

                    sample->owner = block;
                }
            }
        }
    }

    void init_samples_coord(vox_block* block)
    {
        for (u32 i = 0; i < BLOCK_SIZE; ++i)
        {
            for (u32 j = 0; j < BLOCK_SIZE; ++j)
            {
                for (u32 k = 0; k < BLOCK_SIZE; ++k)
                {
                    vox_sample* sample = &(block->samples[i + j * BLOCK_SIZE + k * BLOCK_SIZE_SQUARED]);

                    sample->x = i;
                    sample->y = j;
                    sample->z = k;
                    sample->owner = block;
                }
            }
        }
    }

    void free_block(vox_block* block)
    {
        // unload vao
        for (u32 i = 0; i < vox_block::CHILD_COUNT; ++i)
        {
            if (block->children[i])
            {
                free_block(block->children[i]);
                delete block->children[i];
            }
        }
    }

    std::string save_block(vox_block* block, const std::string& file_name)
    {
        if (block->LOD == 0)
        {
            const u32 buffer_size = 32'768;
            u8 buffer[buffer_size];
            u32 actual_size = encode_block(block, buffer, buffer_size);

            char name_buf[16];
            i32 n = sprintf(name_buf, ".%d_%d_%d.vox", block->position.x, block->position.y, block->position.z);
            std::string complete_name = file_name + name_buf;
            core::file::write(complete_name, buffer, actual_size);
            return complete_name;
        }

        for (u32 i = 0; i < vox_block::CHILD_COUNT; ++i)
        {
            if (block->children[i])
            {
                return save_block(block->children[i], file_name);
            }
        }
    }

    void load_block(vox_block* block, const std::string& file_name)
    {
        assert(block->LOD == LOD_0 && "Cannot load a block of low level of detail.");

        const u32 size = 32'768;
        u8 buffer[size];
        const u32 actual_size = core::file::read(file_name, buffer, size);

        decode_block(block, buffer, actual_size);
    }

    u32 encode_block(vox_block* block, u8* buffer, u32 buffer_size)
    {
        u32 counter = 0;

        core::to_buffer(buffer, buffer_size, &counter, block->position);
        core::to_buffer(buffer, buffer_size, &counter, block->LOD);
        u32 size_counter = counter;
        core::to_buffer(buffer, buffer_size, &counter, u32(0));

        u32 rle_elem_count = 0;
        for (u32 i = 0; i < BLOCK_SIZE_CUBED; ++i, ++rle_elem_count)
        {
            u16 count = 1;
            while (block->samples[i].dist == block->samples[i + 1].dist)
            {
                count++;
                i++;
            }

            core::to_buffer(buffer, buffer_size, &counter, count);
            core::to_buffer(buffer, buffer_size, &counter, block->samples[i].dist);
        }
        core::to_buffer(buffer, buffer_size, &size_counter, rle_elem_count);

        return counter;
    }

    void decode_block(vox_block* block, u8* buffer, u32 buffer_size)
    {
        u32 counter = 0;

        core::from_buffer(buffer, buffer_size, &counter, &block->position);
        core::from_buffer(buffer, buffer_size, &counter, &block->LOD);
        u32 rle_elem_count = 0;
        core::from_buffer(buffer, buffer_size, &counter, &rle_elem_count);

        u32 current_index = 0;
        for (u32 i = 0; i < rle_elem_count; ++i)
        {
            u16 count = 1;
            u8 dist = 0;
            core::from_buffer(buffer, buffer_size, &counter, &count);
            core::from_buffer(buffer, buffer_size, &counter, &dist);

            for (u32 j = 0; j < count; ++j)
            {
                block->samples[current_index].dist = dist;
                current_index++;
            }
        }

        init_samples_coord(block);
    }

    vox_position get_max_size(vox_block* block)
    {
        vox_position position{};

        if (block)
        {
            position = block->position;
            for (u32 i = 0; i < vox_block::CHILD_COUNT; ++i)
            {
                vox_position max_in_children = get_max_size(block->children[i]);

                position.x = std::max(max_in_children.x, position.x);
                position.y = std::max(max_in_children.y, position.y);
                position.z = std::max(max_in_children.z, position.z);
            }
        }
        return position;
    }
}
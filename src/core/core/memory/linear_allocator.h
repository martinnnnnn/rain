#pragma once


#include <cassert>

#include "core/types.h"


namespace rain::core::memory
{
    struct LinearAllocator
    {
        void* first_address;
        u32 capacity;
        u32 memory_used;
        u32 allocations_count;
    };

    void init_linear_allocator(LinearAllocator * const _allocator, void * const _memory, const u32 _capacity);
    void clear_allocator(LinearAllocator * const _allocator);
    void* get_memory(LinearAllocator * const _allocator, u32 _memsize, u8 _aligment);
}
#pragma once


#include <cassert>

#include "core/types.h"


namespace rain::core::memory
{
    struct PoolAllocator
    {
        void*   first_address;
        u32     capacity;
        u32     memory_used;
        u32     allocations_count;
        u32     object_size;
        u8      object_alignment;
        void**  free_list;
    };

    void init_pool_allocator(PoolAllocator * const _allocator, void * const _memory, const u32 _capacity, const u32 _objectSize, const u8 _objectAligment);
    void clear_pool_allocator(PoolAllocator * const _allocator);
    void* get_memory(PoolAllocator * const _allocator, u32 _memsize, u8 _aligment);
    void free_memory(PoolAllocator * const _allocator, void * const _mem);
}
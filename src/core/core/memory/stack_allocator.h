#pragma once


#include <cassert>

#include "core/types.h"


namespace rain::core::memory
{
    struct AllocMetaInfo
    {
        u8 adjustment;
    };

    struct StackAllocator
    {
        void* first_address;
        u32 capacity;
        u32 memory_used;
        u32 allocations_count;
    };

    void init_stack_allocator(StackAllocator * const _allocator, void * const _memory, const u32 _capacity);
    void clear_allocator(StackAllocator * const _allocator);
    void free_memory(StackAllocator * const _allocator, void * const _mem);
    void* get_memory(StackAllocator * const _allocator, u32 _memsize, u8 _aligment);
}
#include "pool_allocator.h"

#include "memory_utility.h"

#include <cmath>

namespace rain::core::memory
{
    void init_pool_allocator(PoolAllocator * const _allocator, void * const _memory, const u32 _capacity, const u32 _objectSize, const u8 _objectAligment)
    {
        _allocator->first_address = _memory;
        _allocator->capacity = _capacity;
        _allocator->memory_used = 0;
        _allocator->allocations_count = 0;
        _allocator->object_size = _objectSize;
        _allocator->object_alignment = _objectAligment;
    }

    void clear_pool_allocator(PoolAllocator * const _allocator)
    {
        u8 adjustment = get_adjustment(_allocator->first_address, _allocator->object_alignment);
        size_t numObjects = (size_t)floor((_allocator->capacity - adjustment) / _allocator->object_size);

        union
        {
            void* as_void_ptr;
            uptr as_uptr;
        };

        as_void_ptr = (void*)_allocator->first_address;

        // align start address
        as_uptr += adjustment;

        _allocator->free_list = (void**)as_void_ptr;

        void** p = _allocator->free_list;

        for (u32 i = 0; i < (numObjects - 1); ++i)
        {
            *p = (void*)((uptr)p + _allocator->object_size);

            p = (void**)*p;
        }

        *p = nullptr;
    }

    void* get_memory(PoolAllocator * const _allocator)
    {
        if (_allocator->free_list == nullptr)
        {
            return nullptr;
        }

        void* p = _allocator->free_list;

        _allocator->free_list = (void**)(*_allocator->free_list);

        _allocator->memory_used += _allocator->object_size;
        _allocator->allocations_count ++;

        return p;
    }

    void free_memory(PoolAllocator * const _allocator, void * const _mem)
    {
        *(void**)_mem = _allocator->free_list;
        _allocator->free_list = (void**)_mem;

        _allocator->memory_used -= _allocator->object_size;
        _allocator->allocations_count--;
    }

}
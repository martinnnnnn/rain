#include "stack_allocator.h"


#include "memory_utility.h"


namespace rain::core::memory
{

    void init_stack_allocator(StackAllocator * const _allocator, void * const _memory, const u32 _capacity)
    {
        _allocator->first_address = _memory;
        _allocator->capacity = _capacity;
        _allocator->memory_used = 0;
        _allocator->allocations_count = 0;
    }

    void clear_allocator(StackAllocator * const _allocator)
    {
        _allocator->memory_used = 0;
        _allocator->allocations_count = 0;
    }

    void free_memory(StackAllocator * const _allocator, void * const _mem)
    {
        union
        {
            void* as_void_ptr;
            uptr as_uptr;
            AllocMetaInfo* as_meta;
        };

        as_void_ptr = _mem;

        // get meta info
        as_uptr -= sizeof(AllocMetaInfo);

        // free used memory
        _allocator->memory_used -= ((uptr)_allocator->first_address + _allocator->memory_used) - ((uptr)_mem + as_meta->adjustment);

        // decrement allocation count
        _allocator->allocations_count--;
    }

    void* get_memory(StackAllocator * const _allocator, u32 _memsize, u8 _aligment)
    {
        assert(_memsize > 0 && "allocate called with memSize = 0.");

        union
        {
            void* as_void_ptr;
            uptr as_uptr;
            AllocMetaInfo* as_meta;
        };

        as_void_ptr = (void*)_allocator->first_address;

        // current address
        as_uptr += _allocator->memory_used;

        u8 adjustment = get_adjustment(as_void_ptr, _aligment, sizeof(AllocMetaInfo));

        // check if there is enough memory available
        if (_allocator->memory_used + _memsize + adjustment > _allocator->capacity)
        {
            // not enough memory
            return nullptr;
        }

        // store alignment in allocation meta info
        as_meta->adjustment = adjustment;

        // determine aligned memory address
        as_uptr += adjustment;

        // update book keeping
        _allocator->memory_used += _memsize + adjustment;
        _allocator->allocations_count++;

        // return aligned memory address
        return as_void_ptr;
    }

}
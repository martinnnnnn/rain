#pragma once



#include "serializer.h"
#include "serializer_binary.h"


namespace rain::core
{
    template<typename T>
    void to_buffer(u8* buffer, u32 max_size, u32* counter, T value)
    {
        assert(*counter + sizeof(T) <= max_size && "Buffer is too small or too much data.");
        memcpy(buffer + *counter, &(value), sizeof(T));
        (*counter) += sizeof(T);
    }

    template<typename T>
    void from_buffer(u8* buffer, u32 max_size, u32* counter, T* value)
    {
        assert(*counter + sizeof(T) <= max_size && "Buffer is too small or too much data.");
        (*value) = *(T*)(buffer + *counter);
        (*counter) += sizeof(T);
    }
}
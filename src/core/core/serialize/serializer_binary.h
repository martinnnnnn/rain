#pragma once 


#include "serializer.h"
#include "core/types.h"

#include <cassert>


namespace rain::core
{
    struct binary_buffer
    {
        u8* data;
        u32 size;
        u32 index;
    };

    struct serializer_binary : public serializer
    {
        serializer_binary(const serializer_mode io, binary_buffer& buffer);
        virtual ~serializer_binary() {};
        binary_buffer& buffer;

        void serialize(char& value, const serialize_info& info) override;
        void serialize(wchar_t& value, const serialize_info& info) override;
        void serialize(i8& value, const serialize_info& info) override;
        void serialize(i16& value, const serialize_info& info) override;
        void serialize(i32& value, const serialize_info& info) override;
        void serialize(i64& value, const serialize_info& info) override;
        void serialize(u8& value, const serialize_info& info) override;
        void serialize(u16& value, const serialize_info& info) override;
        void serialize(u32& value, const serialize_info& info) override;
        void serialize(u64& value, const serialize_info& info) override;
        void serialize(f32& value, const serialize_info& info) override;
        void serialize(f64& value, const serialize_info& info) override;
        void serialize(bool& value, const serialize_info& info) override;
    };

    template <typename T>
    void serialize_bytes(serializer_binary* s, T& value)
    {
        if (s->io_mode == SERIALIZE_IN)
        {
            assert(s->buffer.index + 4 <= s->buffer.size);
#ifdef BIG_ENDIAN
            assert(false && "big endian serialization not implemented");
            //*((uint32_t*)(s->buffer.data + s->buffer.index)) = bswap(value);
#else // #ifdef BIG_ENDIAN
            *((T*)(s->buffer.data + s->buffer.index)) = value;
#endif // #ifdef BIG_ENDIAN
            s->buffer.index += sizeof(T);
        }
        //else
        {
            assert(s->buffer.index + 4 <= s->buffer.size);
#ifdef BIG_ENDIAN
            assert(false && "big endian serialization  not implemented");
            //value = bswap(*((uint32_t*)(buffer.data + buffer.index)));
#else // #ifdef BIG_ENDIAN
            value = *((T*)(s->buffer.data + s->buffer.index));
#endif // #ifdef BIG_ENDIAN
            s->buffer.index += 4;
        }
    }
}


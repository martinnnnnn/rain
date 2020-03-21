#include "serializer_binary.h"


namespace rain::core
{
    serializer_binary::serializer_binary(const serializer_mode io, binary_buffer& buffer)
        : serializer(io)
        , buffer(buffer)
    {

    }

    void serializer_binary::serialize(char& value, const serialize_info& info)
    {
        serialize_bytes(this, value);
    }

    void serializer_binary::serialize(wchar_t& value, const serialize_info& info)
    {
        serialize_bytes(this, value);
    }

    void serializer_binary::serialize(i8& value, const serialize_info& info)
    {
        serialize_bytes(this, value);
    }

    void serializer_binary::serialize(i16& value, const serialize_info& info)
    {
        serialize_bytes(this, value);
    }

    void serializer_binary::serialize(i32& value, const serialize_info& info)
    {
        serialize_bytes(this, value);
    }

    void serializer_binary::serialize(i64& value, const serialize_info& info)
    {
        serialize_bytes(this, value);
    }

    void serializer_binary::serialize(u8& value, const serialize_info& info)
    {
        serialize_bytes(this, value);
    }

    void serializer_binary::serialize(u16& value, const serialize_info& info)
    {
        serialize_bytes(this, value);
    }

    void serializer_binary::serialize(u32& value, const serialize_info& info)
    {
        serialize_bytes(this, value);
    }

    void serializer_binary::serialize(u64& value, const serialize_info& info)
    {
        serialize_bytes(this, value);
    }

    void serializer_binary::serialize(f32& value, const serialize_info& info)
    {
        serialize_bytes(this, value);
    }

    void serializer_binary::serialize(f64& value, const serialize_info& info)
    {
        serialize_bytes(this, value);
    }

    void serializer_binary::serialize(bool& value, const serialize_info& info)
    {
        serialize_bytes(this, value);
    }
}


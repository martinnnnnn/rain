#include "serializer.h"


namespace rain::core
{
    void serialize(serializer* s, char& value, const serialize_info& info)
    {
        s->serialize(value, info);
    }

    void serialize(serializer* s, wchar_t& value, const serialize_info& info)
    {
        s->serialize(value, info);
    }

    void serialize(serializer* s, i8& value, const serialize_info& info)
    {
        s->serialize(value, info);
    }

    void serialize(serializer* s, i16& value, const serialize_info& info)
    {
        s->serialize(value, info);
    }

    void serialize(serializer* s, i32& value, const serialize_info& info)
    {
        s->serialize(value, info);
    }

    void serialize(serializer* s, i64& value, const serialize_info& info)
    {
        s->serialize(value, info);
    }

    void serialize(serializer* s, u8& value, const serialize_info& info)
    {
        s->serialize(value, info);
    }

    void serialize(serializer* s, u16& value, const serialize_info& info)
    {
        s->serialize(value, info);
    }

    void serialize(serializer* s, u32& value, const serialize_info& info)
    {
        s->serialize(value, info);
    }

    void serialize(serializer* s, u64& value, const serialize_info& info)
    {
        s->serialize(value, info);
    }

    void serialize(serializer* s, f32& value, const serialize_info& info)
    {
        s->serialize(value, info);
    }

    void serialize(serializer* s, f64& value, const serialize_info& info)
    {
        s->serialize(value, info);
    }

    void serialize(serializer* s, bool& value, const serialize_info& info)
    {
        s->serialize(value, info);
    }
}


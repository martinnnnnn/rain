#include "serializer_json.h"


namespace rain::core
{
    serializer_json::serializer_json(const serializer_mode io)
        : serializer(io)
    {

    }

    void serializer_json::serialize(char& value, const serialize_info& info)
    {
        serialize_json(this, value);
    }

    void serializer_json::serialize(wchar_t& value, const serialize_info& info)
    {
        serialize_json(this, value);
    }

    void serializer_json::serialize(i8& value, const serialize_info& info)
    {
        serialize_json(this, value);
    }

    void serializer_json::serialize(i16& value, const serialize_info& info)
    {
        serialize_json(this, value);
    }

    void serializer_json::serialize(i32& value, const serialize_info& info)
    {
        serialize_json(this, value);
    }

    void serializer_json::serialize(i64& value, const serialize_info& info)
    {
        serialize_json(this, value);
    }

    void serializer_json::serialize(u8& value, const serialize_info& info)
    {
        serialize_json(this, value);
    }

    void serializer_json::serialize(u16& value, const serialize_info& info)
    {
        serialize_json(this, value);
    }

    void serializer_json::serialize(u32& value, const serialize_info& info)
    {
        serialize_json(this, value);
    }

    void serializer_json::serialize(u64& value, const serialize_info& info)
    {
        serialize_json(this, value);
    }

    void serializer_json::serialize(f32& value, const serialize_info& info)
    {
        serialize_json(this, value);
    }

    void serializer_json::serialize(f64& value, const serialize_info& info)
    {
        serialize_json(this, value);
    }

    void serializer_json::serialize(bool& value, const serialize_info& info)
    {
        serialize_json(this, value);
    }
}


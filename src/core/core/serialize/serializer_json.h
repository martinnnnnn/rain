#pragma once 


#include "serializer.h"
#include "core/types.h"

#include <cassert>


namespace rain::core
{
    struct serializer_json : public serializer
    {
        serializer_json(const serializer_mode io);
        virtual ~serializer_json() {};

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
    void serialize_json(serializer_json* s, T& value)
    {
        if (s->io_mode == SERIALIZE_IN)
        {

        }
        else
        {
        }
    }
}


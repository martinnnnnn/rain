#pragma once


#include <string>

#include "core/types.h"
#include <vector>
namespace rain::core
{
    enum serializer_mode
    {
        SERIALIZE_IN,
        SERIALIZE_OUT
    };

    struct serialize_info
    {
        std::vector<std::string> parents;
        std::string name;
    };

    struct serializer
    {

        serializer_mode io_mode;

        serializer(const serializer_mode io) : io_mode(io) {}
        virtual ~serializer() {}

        virtual void serialize(char& value, const serialize_info& info) = 0;        
        virtual void serialize(wchar_t& value, const serialize_info& info) = 0;
        virtual void serialize(i8& value, const serialize_info& info) = 0;
        virtual void serialize(i16& value, const serialize_info& info) = 0;
        virtual void serialize(i32& value, const serialize_info& info) = 0;
        virtual void serialize(i64& value, const serialize_info& info) = 0;
        virtual void serialize(u8& value, const serialize_info& info) = 0;
        virtual void serialize(u16& value, const serialize_info& info) = 0;
        virtual void serialize(u32& value, const serialize_info& info) = 0;
        virtual void serialize(u64& value, const serialize_info& info) = 0;
        virtual void serialize(f32& value, const serialize_info& info) = 0;
        virtual void serialize(f64& value, const serialize_info& info) = 0;
        virtual void serialize(bool& value, const serialize_info& info) = 0;
    };

    void serialize(serializer* s, char& value, const serialize_info& info);
    void serialize(serializer* s, wchar_t& value, const serialize_info& info);
    void serialize(serializer* s, i8& value, const serialize_info& info);
    void serialize(serializer* s, i16& value, const serialize_info& info);
    void serialize(serializer* s, i32& value, const serialize_info& info);
    void serialize(serializer* s, i64& value, const serialize_info& info);
    void serialize(serializer* s, u8& value, const serialize_info& info);
    void serialize(serializer* s, u16& value, const serialize_info& info);
    void serialize(serializer* s, u32& value, const serialize_info& info);
    void serialize(serializer* s, u64& value, const serialize_info& info);
    void serialize(serializer* s, f32& value, const serialize_info& info);
    void serialize(serializer* s, f64& value, const serialize_info& info);
    void serialize(serializer* s, bool& value, const serialize_info& info);
}


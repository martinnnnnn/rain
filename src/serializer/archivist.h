#pragma once

#include <string>
#include <fstream>
#include <vector>

#include "serializer/var_info.h"
#include "core/types.h"

namespace rain
{



    struct archivist
    {
        enum class IO
        {
            WRITE,
            READ
        };

        archivist(IO _io) :
            io(_io)
        {}

        virtual ~archivist() {}

        virtual void archive(char& _val, const var_info& _info) = 0;
        virtual void archive(wchar_t& _val, const var_info& _info) = 0;
        virtual void archive(std::string& _val, const var_info& _info) = 0;
        virtual void archive(i8& _val, const var_info& _info) = 0;
        virtual void archive(i16& _val, const var_info& _info) = 0;
        virtual void archive(i32& _val, const var_info& _info) = 0;
        virtual void archive(i64& _val, const var_info& _info) = 0;
        virtual void archive(u8& _val, const var_info& _info) = 0;
        virtual void archive(u16& _val, const var_info& _info) = 0;
        virtual void archive(u32& _val, const var_info& _info) = 0;
        virtual void archive(u64& _val, const var_info& _info) = 0;
        virtual void archive(f32& _val, const var_info& _info) = 0;
        virtual void archive(f64& _val, const var_info& _info) = 0;
        virtual void archive(f128& _val, const var_info& _info) = 0;
        virtual void archive(bool& _val, const var_info& _info) = 0;

        IO io;
    };

    void archive(archivist* _p, char& _val, const var_info& _info);
    void archive(archivist* _p, wchar_t& _val, const var_info& _info);
    void archive(archivist* _p, std::string& _val, const var_info& _info);
    void archive(archivist* _p, i8& _val, const var_info& _info);
    void archive(archivist* _p, i16& _val, const var_info& _info);
    void archive(archivist* _p, i32& _val, const var_info& _info);
    void archive(archivist* _p, i64& _val, const var_info& _info);
    void archive(archivist* _p, u8& _val, const var_info& _info);
    void archive(archivist* _p, u16& _val, const var_info& _info);
    void archive(archivist* _p, u32& _val, const var_info& _info);
    void archive(archivist* _p, u64& _val, const var_info& _info);
    void archive(archivist* _p, f32& _val, const var_info& _info);
    void archive(archivist* _p, f64& _val, const var_info& _info);
    void archive(archivist* _p, f128& _val, const var_info& _info);
    void archive(archivist* _p, bool& _val, const var_info& _info);

    //template<typename T>
    //void pup(pupper* _p, std::vector<T> _vec, const var_info& _info)
    //{
    //    uint32_t size = static_cast<uint32_t>(_vec.size());
    //    pup(_p, size, var_info("size"));
    //    vec.resize(size);
    //    for (uint32_t i = 0; i < size; ++i)
    //    {
    //        _p->pup(_vec[i], _info);
    //        pup(_p, _vec[i], var_info(std::to_string(i)));
    //    }
    //}

}
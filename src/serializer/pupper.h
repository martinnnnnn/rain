#pragma once

#include <string>
#include <fstream>
#include <vector>

#include "serializer/var_info.h"
#include "core/types.h"

namespace rain
{



    struct pupper
    {
        enum class IO
        {
            WRITE,
            READ
        };

        pupper(IO _io) :
            io(_io)
        {}

        virtual ~pupper() {}

        virtual void pup(char& _val, const var_info& _info) = 0;
        virtual void pup(wchar_t& _val, const var_info& _info) = 0;
        virtual void pup(std::string& _val, const var_info& _info) = 0;
        virtual void pup(i8& _val, const var_info& _info) = 0;
        virtual void pup(i16& _val, const var_info& _info) = 0;
        virtual void pup(i32& _val, const var_info& _info) = 0;
        virtual void pup(i64& _val, const var_info& _info) = 0;
        virtual void pup(u8& _val, const var_info& _info) = 0;
        virtual void pup(u16& _val, const var_info& _info) = 0;
        virtual void pup(u32& _val, const var_info& _info) = 0;
        virtual void pup(u64& _val, const var_info& _info) = 0;
        virtual void pup(f32& _val, const var_info& _info) = 0;
        virtual void pup(f64& _val, const var_info& _info) = 0;
        virtual void pup(f128& _val, const var_info& _info) = 0;
        virtual void pup(bool& _val, const var_info& _info) = 0;

        IO io;
    };

    void pup(pupper* _p, char& _val, const var_info& _info);
    void pup(pupper* _p, wchar_t& _val, const var_info& _info);
    void pup(pupper* _p, std::string& _val, const var_info& _info);
    void pup(pupper* _p, i8& _val, const var_info& _info);
    void pup(pupper* _p, i16& _val, const var_info& _info);
    void pup(pupper* _p, i32& _val, const var_info& _info);
    void pup(pupper* _p, i64& _val, const var_info& _info);
    void pup(pupper* _p, u8& _val, const var_info& _info);
    void pup(pupper* _p, u16& _val, const var_info& _info);
    void pup(pupper* _p, u32& _val, const var_info& _info);
    void pup(pupper* _p, u64& _val, const var_info& _info);
    void pup(pupper* _p, f32& _val, const var_info& _info);
    void pup(pupper* _p, f64& _val, const var_info& _info);
    void pup(pupper* _p, f128& _val, const var_info& _info);
    void pup(pupper* _p, bool& _val, const var_info& _info);

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
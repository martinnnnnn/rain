#pragma once

#include <string>
#include <fstream>

#include "core/types.h"

namespace rain
{

    struct var_info
    {
        var_info(const std::string & name_) :
            name(name_)
        {}

        virtual ~var_info() {}

        std::string name;
    };

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

        virtual void pup(char& val_, const var_info& info_) = 0;
        virtual void pup(wchar_t& val_, const var_info& info_) = 0;
        virtual void pup(i8& val_, const var_info& info_) = 0;
        virtual void pup(i16& val_, const var_info& info_) = 0;
        virtual void pup(i32& val_, const var_info& info_) = 0;
        virtual void pup(i64& val_, const var_info& info_) = 0;
        virtual void pup(u8& val_, const var_info& info_) = 0;
        virtual void pup(u16& val_, const var_info& info_) = 0;
        virtual void pup(u32& val_, const var_info& info_) = 0;
        virtual void pup(u64& val_, const var_info& info_) = 0;
        virtual void pup(f32& val_, const var_info& info_) = 0;
        virtual void pup(f64& val_, const var_info& info_) = 0;
        virtual void pup(f128& val_, const var_info& info_) = 0;
        virtual void pup(bool& val_, const var_info& info_) = 0;

        IO io;
    };

    void pup(pupper* p, char& val_, const var_info& info_);
    void pup(pupper* p, wchar_t& val_, const var_info& info_);
    void pup(pupper* p, i8& val_, const var_info& info_);
    void pup(pupper* p, i16& val_, const var_info& info_);
    void pup(pupper* p, i32& val_, const var_info& info_);
    void pup(pupper* p, i64& val_, const var_info& info_);
    void pup(pupper* p, u8& val_, const var_info& info_);
    void pup(pupper* p, u16& val_, const var_info& info_);
    void pup(pupper* p, u32& val_, const var_info& info_);
    void pup(pupper* p, u64& val_, const var_info& info_);
    void pup(pupper* p, f32& val_, const var_info& info_);
    void pup(pupper* p, f64& val_, const var_info& info_);
    void pup(pupper* p, f128& val_, const var_info& info_);
    void pup(pupper* p, bool& val_, const var_info& info_);

}
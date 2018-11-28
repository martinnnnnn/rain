#pragma once

#include "pupper.h"

namespace rain
{

    struct binary_file_pupper : public pupper
    {
        binary_file_pupper(std::fstream& _fstrm, IO _mode);

        void pup(char& _val, const var_info& _info) override;
        void pup(wchar_t& _val, const var_info& _info) override;
        void pup(std::string& _val, const var_info& _info) override;
        void pup(i8& _val, const var_info& _info) override;
        void pup(i16& _val, const var_info& _info) override;
        void pup(i32& _val, const var_info& _info) override;
        void pup(i64& _val, const var_info& _info) override;
        void pup(u8& _val, const var_info& _info) override;
        void pup(u16& _val, const var_info& _info) override;
        void pup(u32& _val, const var_info& _info) override;
        void pup(u64& _val, const var_info& _info) override;
        void pup(f32& _val, const var_info& _info) override;
        void pup(f64& _val, const var_info& _info) override;
        void pup(f128& _val, const var_info& _info) override;
        void pup(bool& _val, const var_info& _info) override;

        std::fstream& fs;
    };

    template <class T>
    void pup_bytes(binary_file_pupper* p, T& val_)
    {
        pup_bytes(p, val_, sizeof(T));
    }

    template <class T>
    void pup_bytes(binary_file_pupper* p, T& val_, u32 _size)
    {
        if (p->io == pupper::IO::READ)
        {
            p->fs.read((char*)&val_, _size);
        }
        else
            p->fs.write((char*)&val_, _size);
    }

    struct Test
    {
        i32 hello_i;
        u64 hello_u;
        f32 hello_f;
    };

    void pup(pupper* p_, Test& _test, const var_info& info);
}
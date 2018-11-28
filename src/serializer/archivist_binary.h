#pragma once

#include "archivist.h"

namespace rain
{

    struct archivist_binary : public archivist
    {
        archivist_binary(std::fstream& _fstrm, IO _mode);

        void archive(char& _val, const var_info& _info) override;
        void archive(wchar_t& _val, const var_info& _info) override;
        void archive(std::string& _val, const var_info& _info) override;
        void archive(i8& _val, const var_info& _info) override;
        void archive(i16& _val, const var_info& _info) override;
        void archive(i32& _val, const var_info& _info) override;
        void archive(i64& _val, const var_info& _info) override;
        void archive(u8& _val, const var_info& _info) override;
        void archive(u16& _val, const var_info& _info) override;
        void archive(u32& _val, const var_info& _info) override;
        void archive(u64& _val, const var_info& _info) override;
        void archive(f32& _val, const var_info& _info) override;
        void archive(f64& _val, const var_info& _info) override;
        void archive(f128& _val, const var_info& _info) override;
        void archive(bool& _val, const var_info& _info) override;

        std::fstream& fs;
    };

    template <class T>
    void archive_bytes(archivist_binary* p, T& val_)
    {
        archive_bytes(p, val_, sizeof(T));
    }

    template <class T>
    void archive_bytes(archivist_binary* p, T& val_, u32 _size)
    {
        if (p->io == archivist::IO::READ)
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

    void archive(archivist* p_, Test& _test, const var_info& info);
}
#include "archivist_binary.h"


namespace rain
{
    void archive(archivist * p_, Test & _test, const var_info & info)
    {
        archive(p_, _test.hello_i, info);
        archive(p_, _test.hello_u, info);
        archive(p_, _test.hello_f, info);
    }

    archivist_binary::archivist_binary(std::fstream& _fstream, IO _mode) :
        archivist(_mode),
        fs(_fstream)
    {

    }

    void archivist_binary::archive(char& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void archivist_binary::archive(wchar_t& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void archivist_binary::archive(std::string& _val, const var_info& _info)
    {
        archive_bytes(this, _val, _val.size());
    }

    void archivist_binary::archive(i8& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void archivist_binary::archive(i16& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void archivist_binary::archive(i32& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void archivist_binary::archive(i64& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void archivist_binary::archive(u8& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void archivist_binary::archive(u16& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void archivist_binary::archive(u32& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void archivist_binary::archive(u64& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void archivist_binary::archive(f32& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void archivist_binary::archive(f64& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void archivist_binary::archive(f128& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void archivist_binary::archive(bool& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }
}
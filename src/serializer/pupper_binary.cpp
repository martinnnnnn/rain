#include "pupper_binary.h"


namespace rain
{
    void pup(pupper * p_, Test & _test, const var_info & info)
    {
        pup(p_, _test.hello_i, info);
        pup(p_, _test.hello_u, info);
        pup(p_, _test.hello_f, info);
    }

    binary_file_pupper::binary_file_pupper(std::fstream& _fstream, IO _mode) :
        pupper(_mode),
        fs(_fstream)
    {

    }

    void binary_file_pupper::pup(char& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }

    void binary_file_pupper::pup(wchar_t& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }

    void binary_file_pupper::pup(std::string& _val, const var_info& _info)
    {
        pup_bytes(this, _val, _val.size());
    }

    void binary_file_pupper::pup(i8& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }

    void binary_file_pupper::pup(i16& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }

    void binary_file_pupper::pup(i32& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }

    void binary_file_pupper::pup(i64& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }

    void binary_file_pupper::pup(u8& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }

    void binary_file_pupper::pup(u16& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }

    void binary_file_pupper::pup(u32& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }

    void binary_file_pupper::pup(u64& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }

    void binary_file_pupper::pup(f32& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }

    void binary_file_pupper::pup(f64& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }

    void binary_file_pupper::pup(f128& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }

    void binary_file_pupper::pup(bool& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }
}
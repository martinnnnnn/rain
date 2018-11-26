#include "pupper_binary.h"


namespace rain
{
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

    void binary_file_pupper::pup(int8_t& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }

    void binary_file_pupper::pup(int16_t& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }

    void binary_file_pupper::pup(int32_t& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }

    void binary_file_pupper::pup(int64_t& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }

    void binary_file_pupper::pup(uint8_t& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }

    void binary_file_pupper::pup(uint16_t& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }

    void binary_file_pupper::pup(uint32_t& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }

    void binary_file_pupper::pup(uint64_t& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }

    void binary_file_pupper::pup(float& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }

    void binary_file_pupper::pup(double& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }

    void binary_file_pupper::pup(long double& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }

    void binary_file_pupper::pup(bool& _val, const var_info& _info)
    {
        pup_bytes(this, _val);
    }

}
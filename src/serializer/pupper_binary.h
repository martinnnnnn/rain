#pragma once

#include "pupper.h"

namespace rain
{

    struct binary_file_pupper : public pupper
    {
        binary_file_pupper(std::fstream& _fstrm, IO _mode);

        void pup(char& _val, const var_info& _info);
        void pup(wchar_t& _val, const var_info& _info);
        void pup(int8_t& _val, const var_info& _info);
        void pup(int16_t& _val, const var_info& _info);
        void pup(int32_t& _val, const var_info& _info);
        void pup(int64_t& _val, const var_info& _info);
        void pup(uint8_t& _val, const var_info& _info);
        void pup(uint16_t& _val, const var_info& _info);
        void pup(uint32_t& _val, const var_info& _info);
        void pup(uint64_t& _val, const var_info& _info);
        void pup(float& _val, const var_info& _info);
        void pup(double& _val, const var_info& _info);
        void pup(long double& _val, const var_info& _info);
        void pup(bool& _val, const var_info& _info);

        std::fstream& fs;
    };

    template <class T>
    void pup_bytes(binary_file_pupper* p, T& val_)
    {
        if (p->io == IO::READ)
            p->fs.read((char*)&val_, sizeof(T));
        else
            p->fs.write((char*)&val_, sizeof(T));
    }
}
#pragma once

#include <rapidjson/document.h>
//#include <rapidjson/writer.h>
//#include <rapidjson/stringbuffer.h>

#include "pupper.h"

namespace rain
{

    struct json_pupper : public pupper
    {
        json_pupper(const char* _json, IO _mode);

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

        rapidjson::Document document;

    };


    template<typename T>
    void pup_json(json_pupper* _p, T& _value, const var_info& _info, rapidjson::Value& _json)
    {
        if (_p->io == pupper::IO::READ && _p->document.HasMember(_info.name.c_str()))
        {
            _json = _p->document[_info.name.c_str()];
        }
        else if (_p->io == pupper::IO::WRITE)
        {
            //_json = rapidjson::Value(_info.name, _value);
        }
    }


    //template<typename T>
    //void pup(pupper* _p, std::vector<T>& _vec, const var_info& _info)
    //{
    //    u32 size = static_cast<u32>(_vec.size());
    //    pup(_p, size, var_info("size"));
    //    for (u32 i = 0; i < _json.Size(); i++)
    //    {
    //        position[i] = _json[i].GetFloat();
    //    }

    //    return position;
    //}

}
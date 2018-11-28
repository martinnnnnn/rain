#pragma once

#include <rapidjson/document.h>
//#include <rapidjson/writer.h>
//#include <rapidjson/stringbuffer.h>

#include "archivist.h"

namespace rain
{

    struct archivist_json : public archivist
    {
        archivist_json(const char* _content, IO _mode);

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

        rapidjson::Document document;

    };


    template<typename T>
    void archive_json(archivist_json* _p, T& _value, const var_info& _info, rapidjson::Value& _json)
    {
        if (_p->io == archivist::IO::READ && _p->document.HasMember(_info.name.c_str()))
        {
            _json = _p->document[_info.name.c_str()];
        }
        else if (_p->io == archivist::IO::WRITE)
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
#include "pupper_json.h"





namespace rain
{
    json_pupper::json_pupper(const char* _content, IO _mode) :
        pupper(_mode)
    {
        /*std::string content;
        _fstream.seekg(0, std::ios::end);
        content.reserve(_fstream.tellg());
        _fstream.seekg(0, std::ios::beg);

        content.assign((std::istreambuf_iterator<char>(_fstream)),
            std::istreambuf_iterator<char>());*/

        document.Parse(_content);
    }

    void json_pupper::pup(char& _val, const var_info& _info)
    {

    }

    void json_pupper::pup(wchar_t& _val, const var_info& _info)
    {
        //pup_json(this, _val, _info);
    }

    void json_pupper::pup(std::string& _val, const var_info& _info)
    {
        rapidjson::Value json;
        pup_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = json.GetString();
        }
    }

    void json_pupper::pup(i8& _val, const var_info& _info)
    {
        rapidjson::Value json;
        pup_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<i8>(json.GetInt());
        }
    }

    void json_pupper::pup(i16& _val, const var_info& _info)
    {
        rapidjson::Value json;
        pup_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<i16>(json.GetInt());
        }
    }

    void json_pupper::pup(i32& _val, const var_info& _info)
    {
        rapidjson::Value json;
        pup_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<i32>(json.GetInt());
        }
    }

    void json_pupper::pup(i64& _val, const var_info& _info)
    {
        rapidjson::Value json;
        pup_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<i64>(json.GetInt64());
        }
    }

    void json_pupper::pup(u8& _val, const var_info& _info)
    {
        rapidjson::Value json;
        pup_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<u8>(json.GetUint());
        }
    }

    void json_pupper::pup(u16& _val, const var_info& _info)
    {
        rapidjson::Value json;
        pup_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<u16>(json.GetUint());
        }
    }

    void json_pupper::pup(u32& _val, const var_info& _info)
    {
        rapidjson::Value json;
        pup_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<u32>(json.GetUint());
        }
    }

    void json_pupper::pup(u64& _val, const var_info& _info)
    {
        rapidjson::Value json;
        pup_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<u64>(json.GetUint64());
        }
    }

    void json_pupper::pup(f32& _val, const var_info& _info)
    {
        rapidjson::Value json;
        pup_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<f32>(json.GetFloat());
        }
    }

    void json_pupper::pup(f64& _val, const var_info& _info)
    {
        rapidjson::Value json;
        pup_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<f64>(json.GetDouble());
        }
    }

    void json_pupper::pup(f128& _val, const var_info& _info)
    {
        rapidjson::Value json;
        pup_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<f128>(json.GetDouble());
        }
    }

    void json_pupper::pup(bool& _val, const var_info& _info)
    {
        rapidjson::Value json;
        pup_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<bool>(json.GetBool());
        }
    }

}
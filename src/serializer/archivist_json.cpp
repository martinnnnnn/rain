#include "archivist_json.h"





namespace rain
{
    archivist_json::archivist_json(const char* _content, IO _mode) :
        archivist(_mode)
    {
        document.Parse(_content);
    }

    void archivist_json::archive(char& _val, const var_info& _info)
    {

    }

    void archivist_json::archive(wchar_t& _val, const var_info& _info)
    {
        //pup_json(this, _val, _info);
    }

    void archivist_json::archive(std::string& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = json.GetString();
        }
    }

    void archivist_json::archive(i8& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<i8>(json.GetInt());
        }
    }

    void archivist_json::archive(i16& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<i16>(json.GetInt());
        }
    }

    void archivist_json::archive(i32& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<i32>(json.GetInt());
        }
    }

    void archivist_json::archive(i64& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<i64>(json.GetInt64());
        }
    }

    void archivist_json::archive(u8& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<u8>(json.GetUint());
        }
    }

    void archivist_json::archive(u16& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<u16>(json.GetUint());
        }
    }

    void archivist_json::archive(u32& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<u32>(json.GetUint());
        }
    }

    void archivist_json::archive(u64& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<u64>(json.GetUint64());
        }
    }

    void archivist_json::archive(f32& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<f32>(json.GetFloat());
        }
    }

    void archivist_json::archive(f64& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<f64>(json.GetDouble());
        }
    }

    void archivist_json::archive(f128& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<f128>(json.GetDouble());
        }
    }

    void archivist_json::archive(bool& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<bool>(json.GetBool());
        }
    }

}
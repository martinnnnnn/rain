#include "archivist_json.h"





namespace rain
{
    ArchivistJson::ArchivistJson(const char* _content, IO _mode) :
        Archivist(_mode)
    {
        document.Parse(_content);
    }

    void ArchivistJson::archive(char& _val, const var_info& _info)
    {

    }

    void ArchivistJson::archive(wchar_t& _val, const var_info& _info)
    {
        //pup_json(this, _val, _info);
    }

    void ArchivistJson::archive(std::string& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = json.GetString();
        }
    }

    void ArchivistJson::archive(i8& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<i8>(json.GetInt());
        }
    }

    void ArchivistJson::archive(i16& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<i16>(json.GetInt());
        }
    }

    void ArchivistJson::archive(i32& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<i32>(json.GetInt());
        }
    }

    void ArchivistJson::archive(i64& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<i64>(json.GetInt64());
        }
    }

    void ArchivistJson::archive(u8& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<u8>(json.GetUint());
        }
    }

    void ArchivistJson::archive(u16& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<u16>(json.GetUint());
        }
    }

    void ArchivistJson::archive(u32& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<u32>(json.GetUint());
        }
    }

    void ArchivistJson::archive(u64& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<u64>(json.GetUint64());
        }
    }

    void ArchivistJson::archive(f32& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<f32>(json.GetFloat());
        }
    }

    void ArchivistJson::archive(f64& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<f64>(json.GetDouble());
        }
    }

    void ArchivistJson::archive(f128& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<f128>(json.GetDouble());
        }
    }

    void ArchivistJson::archive(bool& _val, const var_info& _info)
    {
        rapidjson::Value json;
        archive_json(this, _val, _info, json);
        if (!json.IsNull())
        {
            _val = static_cast<bool>(json.GetBool());
        }
    }

    void ArchivistJson::archive(glm::vec3& _val, const var_info& _info)
    {

    }

    void ArchivistJson::archive(glm::vec4& _val, const var_info& _info)
    {

    }

    void ArchivistJson::archive(glm::mat4& _val, const var_info& _info)
    {

    }

    void ArchivistJson::archive(glm::quat& _val, const var_info& _info)
    {

    }

}
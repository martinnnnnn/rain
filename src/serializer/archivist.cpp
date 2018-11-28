#include "archivist.h"

namespace rain
{

    void archive(Archivist* _p, char& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(Archivist* _p, wchar_t& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(Archivist* _p, std::string& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(Archivist* _p, i8& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(Archivist* _p, i16& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(Archivist* _p, i32& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(Archivist* _p, i64& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(Archivist* _p, u8& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(Archivist* _p, u16& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(Archivist* _p, u32& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(Archivist* _p, u64& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(Archivist* _p, f32& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(Archivist* _p, f64& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(Archivist* _p, f128& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(Archivist* _p, bool& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(Archivist* _p, glm::vec3& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }
    void archive(Archivist* _p, glm::vec4& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }
    void archive(Archivist* _p, glm::mat4& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }
    void archive(Archivist* _p, glm::quat& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }
}
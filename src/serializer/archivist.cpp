#include "archivist.h"

namespace rain
{

    void archive(archivist* _p, char& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(archivist* _p, wchar_t& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(archivist* _p, std::string& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(archivist* _p, i8& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(archivist* _p, i16& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(archivist* _p, i32& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(archivist* _p, i64& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(archivist* _p, u8& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(archivist* _p, u16& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(archivist* _p, u32& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(archivist* _p, u64& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(archivist* _p, f32& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(archivist* _p, f64& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(archivist* _p, f128& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }

    void archive(archivist* _p, bool& _val, const var_info& _info)
    {
        _p->archive(_val, _info);
    }
}
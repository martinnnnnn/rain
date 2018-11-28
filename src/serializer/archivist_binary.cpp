#include "archivist_binary.h"


namespace rain
{
    void archive(Archivist * p_, Test & _test, const var_info & info)
    {
        archive(p_, _test.hello_i, info);
        archive(p_, _test.hello_u, info);
        archive(p_, _test.hello_f, info);
    }

    ArchivistBinary::ArchivistBinary(std::fstream& _fstream, IO _mode) :
        Archivist(_mode),
        fs(_fstream)
    {

    }

    void ArchivistBinary::archive(char& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void ArchivistBinary::archive(wchar_t& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void ArchivistBinary::archive(std::string& _val, const var_info& _info)
    {
        archive_bytes(this, _val, _val.size());
    }

    void ArchivistBinary::archive(i8& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void ArchivistBinary::archive(i16& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void ArchivistBinary::archive(i32& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void ArchivistBinary::archive(i64& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void ArchivistBinary::archive(u8& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void ArchivistBinary::archive(u16& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void ArchivistBinary::archive(u32& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void ArchivistBinary::archive(u64& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void ArchivistBinary::archive(f32& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void ArchivistBinary::archive(f64& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void ArchivistBinary::archive(f128& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void ArchivistBinary::archive(bool& _val, const var_info& _info)
    {
        archive_bytes(this, _val);
    }

    void ArchivistBinary::archive(glm::vec3& _val, const var_info& _info)
    {
        archive_bytes(this, _val.x);
        archive_bytes(this, _val.y);
        archive_bytes(this, _val.z);
    }
    void ArchivistBinary::archive(glm::vec4& _val, const var_info& _info)
    {
        archive_bytes(this, _val.x);
        archive_bytes(this, _val.y);
        archive_bytes(this, _val.z);
        archive_bytes(this, _val.w);
    }
    void ArchivistBinary::archive(glm::mat4& _val, const var_info& _info)
    {

    }
    void ArchivistBinary::archive(glm::quat& _val, const var_info& _info)
    {

    }
}
#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "serializer/var_info.h"
#include "core/types.h"

namespace rain
{



    struct Archivist
    {
        enum class IO
        {
            WRITE,
            READ
        };

        Archivist(IO _io) :
            io(_io)
        {}

        virtual ~Archivist() {}

        virtual void archive(char& _val, const var_info& _info) = 0;
        virtual void archive(wchar_t& _val, const var_info& _info) = 0;
        virtual void archive(std::string& _val, const var_info& _info) = 0;
        virtual void archive(i8& _val, const var_info& _info) = 0;
        virtual void archive(i16& _val, const var_info& _info) = 0;
        virtual void archive(i32& _val, const var_info& _info) = 0;
        virtual void archive(i64& _val, const var_info& _info) = 0;
        virtual void archive(u8& _val, const var_info& _info) = 0;
        virtual void archive(u16& _val, const var_info& _info) = 0;
        virtual void archive(u32& _val, const var_info& _info) = 0;
        virtual void archive(u64& _val, const var_info& _info) = 0;
        virtual void archive(f32& _val, const var_info& _info) = 0;
        virtual void archive(f64& _val, const var_info& _info) = 0;
        virtual void archive(f128& _val, const var_info& _info) = 0;
        virtual void archive(bool& _val, const var_info& _info) = 0;
        virtual void archive(glm::vec3& _val, const var_info& _info) = 0;
        virtual void archive(glm::vec4& _val, const var_info& _info) = 0;
        virtual void archive(glm::mat4& _val, const var_info& _info) = 0;
        virtual void archive(glm::quat& _val, const var_info& _info) = 0;

        IO io;
    };

    void archive(Archivist* _p, char& _val, const var_info& _info);
    void archive(Archivist* _p, wchar_t& _val, const var_info& _info);
    void archive(Archivist* _p, std::string& _val, const var_info& _info);
    void archive(Archivist* _p, i8& _val, const var_info& _info);
    void archive(Archivist* _p, i16& _val, const var_info& _info);
    void archive(Archivist* _p, i32& _val, const var_info& _info);
    void archive(Archivist* _p, i64& _val, const var_info& _info);
    void archive(Archivist* _p, u8& _val, const var_info& _info);
    void archive(Archivist* _p, u16& _val, const var_info& _info);
    void archive(Archivist* _p, u32& _val, const var_info& _info);
    void archive(Archivist* _p, u64& _val, const var_info& _info);
    void archive(Archivist* _p, f32& _val, const var_info& _info);
    void archive(Archivist* _p, f64& _val, const var_info& _info);
    void archive(Archivist* _p, f128& _val, const var_info& _info);
    void archive(Archivist* _p, bool& _val, const var_info& _info);
    void archive(Archivist* _p, glm::vec3& _val, const var_info& _info);
    void archive(Archivist* _p, glm::vec4& _val, const var_info& _info);
    void archive(Archivist* _p, glm::mat4& _val, const var_info& _info);
    void archive(Archivist* _p, glm::quat& _val, const var_info& _info);

    template<typename T>
    void archive(Archivist* _p, std::vector<T>& _vec, const var_info& _info)
    {
        _vec.resize(_info.size);
        for (u32 i = 0; _info.size, ++i)
        {
            _p->archive(_vec[i], var_info(""));
        }
    }
}

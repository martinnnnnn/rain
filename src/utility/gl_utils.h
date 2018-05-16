#pragma once

#include "incl_3D.h"

#include <string>
#include <unordered_map>

namespace rain
{
    class GLUtils
    {
    public:
        static std::string GLTypeToString(GLenum _enum);
		static GLenum StringToGLType(const std::string& _name);

	private:
		static std::unordered_map<std::string, GLenum> m_stringToGLType;
    };
}
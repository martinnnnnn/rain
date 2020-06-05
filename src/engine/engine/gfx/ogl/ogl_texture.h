#pragma once

#include "GL/glew.h"
#include "gl/GL.h"
#include "gl/GLU.h"

#include <string>

namespace rain::engine
{
    struct Texture
    {
        std::string file_name;
        GLenum target;
        GLuint id;
        bool load(const std::string& file_name, const GLenum t);
        void bind(GLenum unit);
    };
}
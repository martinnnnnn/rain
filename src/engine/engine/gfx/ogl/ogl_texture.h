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
    };

    bool load_texture(const std::string& file_name, const GLenum t, Texture* texture);
    void bind_texture(Texture const * const texture, GLenum unit);
}
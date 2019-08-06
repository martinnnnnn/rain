#include "ogl_texture.h"

#include "stb_image.h"

#include "core/core.h"
#include "engine/core/context.h"

namespace rain::engine
{
    bool Texture::load(const std::string& file_name, const GLenum t)
    {
        target = t;
        this->file_name = file_name;

        i32 width, height, components_count;
        unsigned char *data = stbi_load(file_name.c_str(), &width, &height, &components_count, 0);
        if (data)
        {
            GLenum format;
            if (components_count == 1)
                format = GL_RED;
            else if (components_count == 3)
                format = GL_RGB;
            else if (components_count == 4)
                format = GL_RGBA;

            glGenTextures(1, &id);
            glBindTexture(target, id);
            glTexImage2D(target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(target);

            glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glBindTexture(target, 0);

            stbi_image_free(data);
            return true;
        }
        else
        {
            RAIN_LOG_ERROR("Texture failed to load at path: %s", file_name.c_str());
            stbi_image_free(data);
            return false;
        }
    }

    bool load_texture(const std::string& file_name, const GLenum t, Texture* texture)
    {
        texture->target = t;
        texture->file_name = file_name;

        i32 width, height, components_count;
        unsigned char *data = stbi_load(file_name.c_str(), &width, &height, &components_count, 0);
        if (data)
        {
            GLenum format;
            if (components_count == 1)
                format = GL_RED;
            else if (components_count == 3)
                format = GL_RGB;
            else if (components_count == 4)
                format = GL_RGBA;

            glGenTextures(1, &texture->id);
            glBindTexture(texture->target, texture->id);
            glTexImage2D(texture->target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(texture->target);

            glTexParameteri(texture->target, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(texture->target, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(texture->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glBindTexture(texture->target, 0);

            stbi_image_free(data);
            return true;
        }
        else
        {
            RAIN_LOG_ERROR("Texture failed to load at path: %s", texture->file_name.c_str());
            stbi_image_free(data);
            return false;
        }
    }

    void bind_texture(Texture const * const texture, GLenum unit)
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(texture->target, texture->id);
    }
}
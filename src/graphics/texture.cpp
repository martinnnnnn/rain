#include "texture.h"


#include <stb_image.h>


namespace rain
{
    namespace TextureUtils
    {
        GLint ChannelCountToFormat(int _channelCount)
        {
            switch (_channelCount)
            {
            case 1:
                return GL_RED;
            case 2:
                return GL_RG;
            case 3:
                return GL_RGB;
            case 4:
                return GL_RGBA;
            default:
                return GL_RGBA;
            }
        }

        std::string Texture2DTypeToString(Texture2DType _type)
        {
            switch (_type)
            {
            case Texture2DType::NONE:
                return "none";
            case Texture2DType::DIFFUSE:
                return "diffuse";
            case Texture2DType::SPECULAR:
                return "specular";
            case Texture2DType::AMBIENT:
                return "ambient";
            case Texture2DType::EMISSIVE:
                return "emissive";
            case Texture2DType::HEIGHT:
                return "height";
            case Texture2DType::NORMALS:
                return "normals";
            case Texture2DType::SHININESS:
                return "shininess";
            case Texture2DType::OPACITY:
                return "opacity";
            case Texture2DType::DISPLACEMENT:
                return "displacement";
            case Texture2DType::LIGHTMAP:
                return "lightmap";
            case Texture2DType::REFLECTION:
                return "reflection";
            case Texture2DType::UNKNOWN:
                return "unknown";
            }
            return "unknown";
        }
    }

    //TODO(martin) : error handling
	Texture2D Load2DTexture(const std::string& _path, Texture2DType _type, bool _flipVertically)
	{
        Texture2D texture{};
        texture.path = _path;
        texture.type = _type;
        
		glGenTextures(1, &texture.id);
		glBindTexture(GL_TEXTURE_2D, texture.id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_set_flip_vertically_on_load(_flipVertically);

		int width, height, channelCount;
		unsigned char * data = stbi_load((texture.path).c_str(), &width, &height, &channelCount, 0);
		if (data)
		{
            texture.format = TextureUtils::ChannelCountToFormat(channelCount);
			glTexImage2D(GL_TEXTURE_2D, 0, texture.format, width, height, 0, texture.format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture at : " << (texture.path) << std::endl;
            return Texture2D{};
		}
		stbi_image_free(data);
        return texture;
	}

	Texture2D Load2DTextureWithData(const std::string& _path, Texture2DType _type, const Texture2DData& data)
	{
		Texture2D texture{};
		texture.path = _path;
		texture.type = _type;
		texture.format = data.format;
		
		glGenTextures(1, &texture.id);
		glBindTexture(GL_TEXTURE_2D, texture.id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		glTexImage2D(GL_TEXTURE_2D, 0, data.format, data.width, data.height, 0, data.format, GL_UNSIGNED_BYTE, data.data);
		glGenerateMipmap(GL_TEXTURE_2D);

		return texture;
	}

    //TODO(martin) : error handling
	TextureCubeMap LoadTextureCubeMap(std::vector<std::string> _paths, bool _flipVertically)
	{
        TextureCubeMap texture{};
        texture.paths = _paths;

		glGenTextures(1, &texture.id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		
		int width, height, channelCount;
		unsigned char* data;
		stbi_set_flip_vertically_on_load(_flipVertically);
		for (size_t i = 0; i < texture.paths.size(); ++i)
		{
			data = stbi_load(texture.paths[i].c_str(), &width, &height, &channelCount, 0);
			if (data)
			{
                texture.format = TextureUtils::ChannelCountToFormat(channelCount);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, texture.format, width, height, 0, texture.format, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Failed to load Cubemap texture at path: " << texture.paths[i] << std::endl;
			}
		}
		return texture;
	}

    void BindTexture(GLenum _type, GLuint _id)
    {
        glBindTexture(_type, _id);
    }

}

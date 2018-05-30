#pragma once

#include <string>
#include <iostream>
#include <vector>

#include "utility/incl_3d.h"


namespace rain
{
    enum class Texture2DType
    {
        NONE = 0x0,
        DIFFUSE = 0x1,
        SPECULAR = 0x2,
        AMBIENT = 0x3,
        EMISSIVE = 0x4,
        HEIGHT = 0x5,
        NORMALS = 0x6,
        SHININESS = 0x7,
        OPACITY = 0x8,
        DISPLACEMENT = 0x9,
        LIGHTMAP = 0xA,
        REFLECTION = 0xB,
        UNKNOWN = 0xC,
    };

    namespace TextureUtils
    {
        GLint ChannelCountToFormat(int _channelCount);
        std::string Texture2DTypeToString(Texture2DType _type);
    }

	struct Texture2D
	{
        GLuint id;
        std::string path;
        Texture2DType type;
        GLint format;
	};

	struct TextureCubeMap
	{
        GLuint id;
		std::vector<std::string> paths;
        Texture2DType type;
        GLint format;
	};

    Texture2D LoadTexture2D(const std::string& _path, Texture2DType _type, bool _flipVertically = false);
	TextureCubeMap LoadTextureCubeMap(std::vector<std::string> _paths, bool _flipVertically = false);
    void BindTexture(GLenum _type, GLuint _id);

}
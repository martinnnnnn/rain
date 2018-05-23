#pragma once

#include <string>
#include <iostream>
#include <vector>

#include "utility/incl_3d.h"
//#include "utility/img_utils.h"
#include <stb_image.h>


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

	class Texture
	{
	public:
		Texture() {}

		virtual void Bind() = 0;

	public:
		GLuint id;
        std::string type;
	protected:
		GLint m_format;
	};

	class Texture2D : public Texture
	{
	public:
		Texture2D() : Texture() {}

		int Load(const std::string& _path, Texture2DType _type, bool _flipVertically = false);
		void Bind() override;

	private: 
		std::string m_path;
        Texture2DType m_type;
	};

	
	class TextureCubeMap : public Texture
	{
	public:
		TextureCubeMap() : Texture() {}

		int Load(std::vector<std::string> _paths, bool _flipVertically = false);
		void Bind() override;
	private:
		std::vector<std::string> m_paths;
	};


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
	}
}
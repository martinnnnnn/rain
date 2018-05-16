#pragma once

#include <string>
#include <iostream>
#include <vector>

#include "utility/incl_3d.h"
//#include "utility/img_utils.h"
#include <stb_image.h>


namespace rain
{
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

		int Load(const std::string& _path, bool _flipVertically = false);
		void Bind() override;

	private: 
		std::string m_path;
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
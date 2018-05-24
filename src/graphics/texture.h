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



	class Texture
	{
	public:
		Texture() {}

        virtual void Bind() {};

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

		int Load(const std::string& _directory, const std::string& _fileName, Texture2DType _type, bool _flipVertically = false);
		void Bind() override;

	private: 
        std::string m_directory;
        std::string m_fileName;
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
}
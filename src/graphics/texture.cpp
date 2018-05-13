#include "texture.h"



namespace rain
{
	int Texture2D::Load(const std::string& _path, bool _flipVertically)
	{
		m_path = _path;

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_set_flip_vertically_on_load(_flipVertically);

		int width, height, channelCount;
		unsigned char * data = stbi_load(m_path.c_str(), &width, &height, &channelCount, 0);
		if (data)
		{
			m_format = TextureUtils::ChannelCountToFormat(channelCount);
			glTexImage2D(GL_TEXTURE_2D, 0, m_format, width, height, 0, m_format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture at : " << m_path << std::endl;
			return -1;
		}
		stbi_image_free(data);
		return 0;
	}

	void Texture2D::Bind()
	{
		glBindTexture(GL_TEXTURE_2D, id);
	}

	int TextureCubeMap::Load(std::vector<std::string> _paths, bool _flipVertically)
	{
		int result = 0;
		m_paths = _paths;

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		
		int width, height, channelCount;
		unsigned char* data;
		stbi_set_flip_vertically_on_load(_flipVertically);
		for (size_t i = 0; i < m_paths.size(); ++i)
		{
			data = stbi_load(m_paths[i].c_str(), &width, &height, &channelCount, 0);
			if (data)
			{
				m_format = TextureUtils::ChannelCountToFormat(channelCount);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_format, width, height, 0, m_format, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				result = -1;
				std::cout << "Failed to load Cubemap texture at path: " << m_paths[i] << std::endl;
			}
		}
		return result;
	}


	void TextureCubeMap::Bind()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	}
}

#pragma once


#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "utility/incl_3d.h"

namespace rain
{
	class Shader;

	struct vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 textCoords;
	};

	struct texture
	{
		GLuint id;
		std::string type;
	};

	class Mesh
	{
	public:
		std::vector<vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<texture> textures;

		Mesh(std::vector<vertex> _vertices, std::vector<unsigned int> _indices, std::vector<texture> _textures);
		void Draw(Shader* _shader);


	private:
		GLuint m_vao;
		GLuint m_vbo;
		GLuint m_ebo;
	};
}
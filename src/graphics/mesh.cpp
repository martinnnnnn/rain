//#include "graphics/mesh.h"
//
//
//#include "shader.h"
//
//
//namespace rain
//{
//	Mesh::Mesh(std::vector<vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture2D> _textures)
//	{
//		vertices = _vertices;
//		indices = _indices;
//		textures = _textures;
//
//		glGenVertexArrays(1, &m_vao);
//		glGenBuffers(1, &m_vbo);
//		glGenBuffers(1, &m_ebo);
//
//		glBindVertexArray(m_vao);
//
//		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
//		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);
//
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
//
//		glEnableVertexAttribArray(0);
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
//
//		glEnableVertexAttribArray(1);
//		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
//
//		glEnableVertexAttribArray(2);
//		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, textCoords));
//
//		glBindVertexArray(0);
//	}
//
//	void Mesh::Draw(Shader* _shader)
//	{
//		unsigned int diffuseNr = 1;
//		unsigned int specularNr = 1;
//
//		for (unsigned int i = 0; i < textures.size(); i++)
//		{
//			glActiveTexture(GL_TEXTURE0 + i);
//			std::string number;
//			std::string name = textures[i].type;
//			if (name == "texture_diffuse")
//				number = std::to_string(diffuseNr++);
//			else if (name == "texture_specular")
//				number = std::to_string(specularNr++);
//
//			_shader->setParameter(("material." + name + number).c_str(), (int)i);
//			glBindTexture(GL_TEXTURE_2D, textures[i].id);
//		}
//		glActiveTexture(GL_TEXTURE0);
//
//		glBindVertexArray(m_vao);
//		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//		glBindVertexArray(0);
//	}
//}
//

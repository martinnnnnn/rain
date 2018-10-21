#pragma once

#include "GL/glew.h"
#include "gl/GL.h"
#include "gl/GLU.h"
#include "glm/glm.hpp"

#include <string>
#include <vector>

#include "core/file.h"


namespace rain
{

	class Shader
	{
	public:
		GLuint id;

		Shader();
		void use();
		void reload();
		bool load(const std::string& _vertex_file, const std::string& _fragment_file, const std::string& _geometry_file = "");
		void load(const char* _vertex_code, const char* _fragment_code, const char* _geometry_code = nullptr);

		void set(const std::string &name, bool value) const;
		void set(const std::string &name, int value) const;
		void set(const std::string &name, float x) const;
		void set(const std::string &name, float x, float y) const;
		void set(const std::string &name, float x, float y, float z) const;
		void set(const std::string &name, float x, float y, float z, float w) const;
		void set(const std::string &name, const glm::fvec2 &vector) const;
		void set(const std::string &name, const glm::fvec3 &vector) const;
		void set(const std::string &name, const glm::fmat4 &matrix);

	private:
		
		//std::string readFile(const char* _path);

		void check_compile_errors(unsigned int _shader, std::string _type);

		File m_vertex_file;
		File m_fragment_file;
		File m_geometry_file;

		GLuint m_vertexShader;
		GLuint m_fragmentShader;
		GLuint m_geometryShader;
	};
}
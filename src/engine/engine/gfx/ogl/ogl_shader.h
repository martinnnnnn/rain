#pragma once

#include "GL/glew.h"
#include "gl/GL.h"
#include "gl/GLU.h"
#include "glm/glm.hpp"

#include <string>
#include <vector>

#include "core/core.h"


namespace rain::engine
{

	class Shader
	{
	public:
		GLuint id;

		Shader();
		void use() const;
		bool load(const std::string& _vertex_file, const std::string& _fragment_file, const std::string& _geometry_file = "");
        void unload();

		u32 set(const std::string& _name, bool _value) const;
		u32 set(const std::string& _name, int _value) const;
		u32 set(const std::string& _name, float _x) const;
		u32 set(const std::string& _name, float _x, float _y) const;
		u32 set(const std::string& _name, float _x, float _y, float _z) const;
		u32 set(const std::string& _name, float _x, float _y, float _z, float _w) const;
		u32 set(const std::string& _name, const glm::fvec2& _vector) const;
		u32 set(const std::string& _name, const glm::fvec3& _vector) const;
		u32 set(const std::string& _name, const glm::fmat4& _matrix) const;

        void set(u32 _parameterID, bool _value) const;
        void set(u32 _parameterID, int _value) const;
        void set(u32 _parameterID, float _x) const;
        void set(u32 _parameterID, float _x, float _y) const;
        void set(u32 _parameterID, float _x, float _y, float _z) const;
        void set(u32 _parameterID, float _x, float _y, float _z, float _w) const;
        void set(u32 _parameterID, const glm::fvec2& _vector) const;
        void set(u32 _parameterID, const glm::fvec3& _vector) const;
        void set(u32 _parameterID, const glm::fmat4& _matrix) const;

	private:
		
		void load(const char* _vertex_code, const char* _fragment_code, const char* _geometry_code = nullptr);
		void check_compile_errors(unsigned int _shader, const std::string& _type);

		core::File m_vertex_file;
		core::File m_fragment_file;
		core::File m_geometry_file;

		GLuint m_vertexShader;
		GLuint m_fragmentShader;
		GLuint m_geometryShader;
	};
}
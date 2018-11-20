#pragma once


#include <glm/glm.hpp>

#include "gfx/ogl/ogl_shader.h"
#include "core/types.h"

namespace rain
{
    class Debug
    {
    public:
        Debug();
        void init();
        void render(const glm::mat4& _view, const glm::mat4& _proj);
        void add_line(const glm::vec3& _point1, const glm::vec3& _point2, const glm::vec3& _color);
        void add_cube(const glm::vec3& _point1, const glm::vec3& _point2);
        void add_sphere(const glm::vec3& _point1, const glm::vec3& _point2);
        void add_quad(const glm::vec3& _point1, const glm::vec3& _point2);

    private:
        static constexpr u32 vertices_max_count = 8192;

        u32      m_vertexCount;
        glm::vec3 m_vertices[vertices_max_count];
        glm::vec3 m_colors[vertices_max_count];
        u32      m_vao;
        u32      m_vbo;
        u32      m_cbo;
        Shader shader;
    };
}
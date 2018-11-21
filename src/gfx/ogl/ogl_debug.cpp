#include "ogl_debug.h"


#include "GL/glew.h"
#include "gl/GL.h"
#include "gl/GLU.h"
#include <glm/gtc/type_ptr.hpp>


#include "core/config.h"

namespace rain
{
    Debug::Debug()
        : m_vertexCount(0)
        , m_vao(0)
        , m_vbo(0)
        , m_cbo(0)
    {}

    void Debug::init()
    {
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices_max_count * sizeof(glm::vec3), &m_vertices[0], GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);

        glGenBuffers(1, &m_cbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_cbo);
        glBufferData(GL_ARRAY_BUFFER, vertices_max_count * sizeof(glm::vec3), &m_colors[0], GL_DYNAMIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);

        glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        shader.load(RAIN_CONFIG->data_root + "/shaders/glsl/debug.vs", RAIN_CONFIG->data_root + "/shaders/glsl/debug.fs");
    }


    void Debug::add_line(const glm::vec3& _point1, const glm::vec3& _point2, const glm::vec3& _color)
    {
        if (m_vertexCount + 2 < vertices_max_count)
        {
            m_vertices[m_vertexCount + 0] = _point1;
            m_vertices[m_vertexCount + 1] = _point2;
            m_colors[m_vertexCount + 0] = _color;
            m_colors[m_vertexCount + 1] = _color;

            m_vertexCount += 2;
        }

    }

    void Debug::add_cube(const glm::vec3& _center, f32 _width, f32 height, const glm::vec3& _color)
    {
        float halfWidth = _width / 2.0f;
        float halfHeight = height / 2.0f;

        glm::vec3 A(_center.x - halfWidth, _center.y + halfWidth, _center.z - halfHeight);
        glm::vec3 B(_center.x + halfWidth, _center.y + halfWidth, _center.z - halfHeight);
        glm::vec3 C(_center.x + halfWidth, _center.y - halfWidth, _center.z - halfHeight);
        glm::vec3 D(_center.x - halfWidth, _center.y - halfWidth, _center.z - halfHeight);
        glm::vec3 E(_center.x - halfWidth, _center.y + halfWidth, _center.z + halfHeight);
        glm::vec3 F(_center.x + halfWidth, _center.y + halfWidth, _center.z + halfHeight);
        glm::vec3 G(_center.x + halfWidth, _center.y - halfWidth, _center.z + halfHeight);
        glm::vec3 H(_center.x - halfWidth, _center.y - halfWidth, _center.z + halfHeight);

        add_line(A, B, _color);
        add_line(B, C, _color);
        add_line(C, D, _color);
        add_line(D, A, _color);

        add_line(A, E, _color);
        add_line(B, F, _color);
        add_line(C, G, _color);
        add_line(D, H, _color);

        add_line(E, F, _color);
        add_line(F, G, _color);
        add_line(G, H, _color);
        add_line(H, E, _color);
    }

    void Debug::add_sphere(const glm::vec3& _point1, const glm::vec3& _point2)
    {

    }

    void Debug::add_quad(const glm::vec3& _point1, const glm::vec3& _point2)
    {

    }

    void Debug::render(const glm::mat4& _view, const glm::mat4& _proj)
    {
        if (m_vertexCount == 0)
        {
            return;
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertexCount * sizeof(glm::vec3), m_vertices);

        glBindBuffer(GL_ARRAY_BUFFER, m_cbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertexCount * sizeof(glm::vec3), m_colors);

        glEnable(GL_MULTISAMPLE);
        glDisable(GL_BLEND);

        // Sets up shader
        glm::mat4 mvp = _proj * _view * glm::mat4(1.0f);
        shader.use();
        shader.set("mvp", mvp);

        // Draws all lines
        glBindVertexArray(m_vao);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glDrawArrays(GL_LINES, 0, m_vertexCount);

        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);
        glBindVertexArray(0);

        glDisable(GL_MULTISAMPLE);
    }
}
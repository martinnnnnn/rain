#pragma once

#include <unordered_map>

#include "GL/glew.h"
#include "gl/GL.h"
#include "gl/GLU.h"

#include "core/core.h"
#include "ogl_shader.h"
#include "engine/core/config.h"
#include "engine/data/geometry/mesh.h"
#include "engine/data/material/material.h"


namespace rain::engine
{
    struct StaticMesh
    {
        GLuint vao;
    };

    struct RChar
    {
        GLuint     texture_id;
        glm::ivec2 size;
        glm::ivec2 bearing;
        GLuint     advance;
    };

    class Renderer
    {
    public:
        void init();

        void clear();


        void init_default_shaders();

        void init_debug();

        void init_shapes();
	    void init_cube();
        void init_sphere();

        void init_text_2d();

        void draw();

        void draw_debug();
        void draw_debug_line(const glm::vec3& _point1, const glm::vec3& _point2, const glm::vec3& _color);
        void draw_debug_line(const glm::vec3& _point1, const glm::vec3& _point2, const glm::vec3& _color1, const glm::vec3& _color2);
        void draw_debug_cube(const glm::vec3& _center, const f32 _width, const f32 _height, const glm::vec3& _color);
        void draw_debug_sphere(const glm::vec3& _position, const f32 _scale = 1.0f, const glm::quat& orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)));
        void draw_coord_view(const glm::vec3& _position);

        void draw_billboard();
        void draw_line();
        void draw_quad(const core::math::Plane& _p, const glm::vec3 _position, const glm::vec3& _color);
        void draw_cube(const glm::vec3& _position, const f32 _scale = 1.0f, const glm::quat& orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)));
        void draw_sphere(const glm::vec3& _position, const f32 _scale = 1.0f, const glm::quat& orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)));
        void draw_mesh();

        void draw_text_2d(const std::string& _text, const f32 _x, const f32 _y, const f32 _scale, const glm::vec3& _color);

        void resize(u32 _width, u32 _height);
        void set_perspective_projection_matrix(const glm::mat4& _projection);
        void set_orthogonal_projection_matrix(const glm::mat4& _projection);
        void set_view_matrix(const glm::vec3& _eye, float _pitch, float _yaw);
        void set_view_matrix(const glm::vec3& _eye, const glm::vec3& _center, const glm::vec3& _up);
        void set_view_matrix(const glm::mat4& _matrix);

        void load_mesh(Mesh* _mesh);
        void draw_mesh(Mesh* _mesh, const Material& material, const glm::vec3& _position, const glm::quat& _orientation, const glm::vec3& _scale);
        void release_mesh(Mesh* _mesh);

        static constexpr u32 debug_vertices_max_count = 8192;

        u32      m_debug_vertex_count;
        glm::vec3 m_debug_vertices[debug_vertices_max_count];
        glm::vec3 m_debug_colors[debug_vertices_max_count];
        u32      m_debug_vao;
        u32      m_debug_vbo;
        u32      m_debug_cbo;
        Shader debug_shader;

        GLuint lineVAO;
        u32 line_index_count;
	    GLuint quadVAO;
	    u32 quad_index_count;
	    GLuint cubeVAO;
	    u32 cube_index_count;
	    GLuint sphereVAO;
	    u32 sphere_index_count;

        glm::mat4 proj_mat_perspective;
        glm::mat4 proj_mat_orthogonal;
        glm::mat4 view_mat;

        Shader phong;
        Shader pbr;

        std::unordered_map<GLchar, RChar> text_characters;
        Shader text_shader;
        GLuint text_vao;
        GLuint text_vbo;
    };
}

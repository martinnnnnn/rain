#pragma once

#include <unordered_map>

#include "GL/glew.h"
#include "gl/GL.h"
#include "gl/GLU.h"

#include "core/core.h"
#include "math/math.h"
#include "ogl_shader.h"
#include "engine/core/config.h"
#include "engine/data/geometry/mesh.h"
#include "engine/data/material/material.h"


namespace rain::engine
{
    using vec3 = math::vec3;
    using quat = math::quat;
    using mat4 = math::mat4;
    using plane = math::Plane;

    struct StaticMesh
    {
        GLuint vao;
    };

    struct RChar
    {
        GLuint     texture_id;
        i32 size[2];
        i32 bearing[2];
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
        void draw_debug_line(const vec3& _point1, const vec3& _point2, const vec3& _color);
        void draw_debug_line(const vec3& _point1, const vec3& _point2, const vec3& _color1, const vec3& _color2);
        void draw_debug_cube(const vec3& _center, const f32 _width, const f32 _height, const vec3& _color);
        void draw_debug_sphere(const vec3& _position, const f32 _scale = 1.0f, const quat& orientation = quat(vec3(0.0f, 0.0f, 0.0f)));
        void draw_coord_view(const vec3& _position);

        void draw_billboard();
        void draw_line();
        void draw_quad(const Plane& _p, const vec3 _position, const vec3& _color);
        void draw_cube(const vec3& _position, const f32 _scale = 1.0f, const quat& orientation = quat(vec3(0.0f, 0.0f, 0.0f)));
        void draw_sphere(const vec3& _position, const f32 _scale = 1.0f, const quat& orientation = quat(vec3(0.0f, 0.0f, 0.0f)));
        void draw_mesh();

        void draw_text_2d(const std::string& _text, const f32 _x, const f32 _y, const f32 _scale, const vec3& _color);

        void resize(u32 _width, u32 _height);
        void set_perspective_projection_matrix(const mat4& _projection);
        void set_orthogonal_projection_matrix(const mat4& _projection);
        void set_view_matrix(const vec3& _eye, float _pitch, float _yaw);
        void set_view_matrix(const vec3& _eye, const vec3& _center, const vec3& _up);
        void set_view_matrix(const mat4& _matrix);

        using vec3 = vec3;
        u32 load_primitive(vec3* vertices, u32 vertices_count, u32* vertices_indices, u32 vertices_indices_count, vec3* normals, u32 normals_count, u32* normals_indices, u32 normals_indices_count);
        void draw_primitive(const u32 vao, const u32 nindices, const Material& material, const vec3& position, const quat& _orientation, const vec3& scale);

        void load_mesh(Mesh* _mesh);
        void draw_mesh(Mesh* _mesh, const Material& material, const vec3& _position, const quat& _orientation, const vec3& _scale);
        void release_mesh(Mesh* _mesh);

        static constexpr u32 debug_vertices_max_count = 8192;

        u32      m_debug_vertex_count;
        vec3 m_debug_vertices[debug_vertices_max_count];
        vec3 m_debug_colors[debug_vertices_max_count];
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

        mat4 proj_mat_perspective;
        mat4 proj_mat_orthogonal;
        mat4 view_mat;

        Shader phong;
        Shader pbr;

        std::unordered_map<GLchar, RChar> text_characters;
        Shader text_shader;
        GLuint text_vao;
        GLuint text_vbo;
    };
}

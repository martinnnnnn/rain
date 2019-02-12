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
#include "engine/data/data_handle.h"

namespace rain::engine
{
    struct StaticMesh
    {
        GLuint vao;
    };

    struct RChar
    {
        GLuint  texture_id;
        i32     size[2];
        i32     bearing[2];
        GLuint  advance;
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
		void init_quad_2d();
		void draw_quad_2d();

        void draw();

        void draw_debug();
        void draw_debug_line(const math::vec3& _point1, const math::vec3& _point2, const math::vec3& _color);
        void draw_debug_line(const math::vec3& _point1, const math::vec3& _point2, const math::vec3& _color1, const math::vec3& _color2);
        void draw_debug_cube(const math::vec3& _center, const f32 _width, const f32 _height, const math::vec3& _color);
        void draw_debug_sphere(const math::vec3& _position, const f32 _scale = 1.0f, const math::quat& orientation = math::quat(math::from_euler()));
        void draw_coord_view(const math::vec3& _position);

        void draw_billboard();
        void draw_line();
        void draw_quad(const math::plane& _p, const math::vec3 _position, const math::vec3& _color);
        void draw_cube(const math::vec3& position, const math::quat& orientation, const math::vec3& scale);
        void draw_sphere(const math::vec3& position, const math::quat& orientation, const math::vec3& scale);
        void draw_mesh();

        void draw_text_2d(const std::string& _text, const f32 _x, const f32 _y, const f32 _scale, const math::vec3& _color);

        void resize(u32 _width, u32 _height);
        void set_perspective_projection_matrix(const math::mat4& _projection);
        void set_orthogonal_projection_matrix(const math::mat4& _projection);
        void set_view_matrix(const math::vec3& _eye, float _pitch, float _yaw);
        void set_view_matrix(const math::vec3& _eye, const math::vec3& _center, const math::vec3& _up);
        void set_view_matrix(const math::mat4& _matrix);

        u32 load_primitive(math::vec3* vertices, u32 vertices_count, u32* vertices_indices, u32 vertices_indices_count, math::vec3* normals, u32 normals_count, u32* normals_indices, u32 normals_indices_count);
        void draw_primitive(const u32 vao, const u32 nindices, const Material& material, const math::vec3& position, const math::quat& _orientation, const math::vec3& scale);

        void load_mesh(Mesh* mesh);
        void draw_mesh(Mesh* mesh, const Material& material, const math::vec3& position, const math::quat& orientation, const math::vec3& scale);
        void release_mesh(Mesh* mesh);

        static constexpr u32 debug_vertices_max_count = 8192;

        u32      m_debug_vertex_count;
        math::vec3 m_debug_vertices[debug_vertices_max_count];
        math::vec3 m_debug_colors[debug_vertices_max_count];
        u32      m_debug_vao;
        u32      m_debug_vbo;
        u32      m_debug_cbo;

		Shader shape_2d_shader;
		u32		quad_2d_vao;
        Shader debug_shader;

        GLuint lineVAO;
        u32 line_index_count;
	    GLuint quadVAO;
	    u32 quad_index_count;
	    GLuint cubeVAO;
	    u32 cube_index_count;
	    GLuint sphereVAO;
	    u32 sphere_index_count;

        math::mat4 proj_mat_perspective;
        math::mat4 proj_mat_orthogonal;
        math::mat4 view_mat;

        Shader debug_shader;
        handle<Shader> const * debug_shader_handle;
        Shader phong_shader;
        Shader pbr_shader;
        Shader text_shader;

        std::unordered_map<GLchar, RChar> text_characters;
        GLuint text_vao;
        GLuint text_vbo;

		static constexpr u32 ui_quad_max_vertex_count = 8192;
		u32 ui_quad_vertex_count;
		u32 ui_quad_vao;
		u32 ui_quad_vbo;
		u32 ui_quad_cbo;
		math::vec3 ui_quad_vertices[ui_quad_max_vertex_count];
		math::vec3 ui_quad_colors[ui_quad_max_vertex_count];
		Shader ui_shader;
		void init_ui();
		void draw_ui();
		void draw_ui_triangle(const math::vec3& a, const math::vec3& b, const math::vec3& c, const math::vec3& a_color);
		void draw_ui_triangle(const math::vec3& a, const math::vec3& b, const math::vec3& c, const math::vec3& a_color, const math::vec3& b_color);
		void draw_ui_triangle(const math::vec3& a, const math::vec3& b, const math::vec3& c, const math::vec3& a_color, const math::vec3& b_color, const math::vec3& c_color);
		void draw_ui_quad(const math::vec3& top_left, const u32& width, const u32& height, const math::vec3& color);
	};
}

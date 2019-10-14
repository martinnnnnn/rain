#pragma once

#include <unordered_map>

#include "GL/glew.h"
#include "gl/GL.h"
#include "gl/GLU.h"

#include "core/core.h"
#include "glm.hpp"
#include "ogl_shader.h"
#include "ogl_texture.h"
#include "engine/core/config.h"
#include "engine/data/geometry/mesh.h"
#include "engine/data/material/material.h"
#include "engine/data/data_handle.h"
#include "engine/voxel/voxel.h"

//#define SPIRV


namespace rain::engine
{

#ifdef SPIRV
    static PFNGLSPECIALIZESHADERPROC gl_specialize_shader;
#endif

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
        void update();
        void clear();

        void init_default_shaders();
        void init_debug();
        void init_shapes();
	    void init_cube();
        void init_sphere();
        void init_instancing(const std::vector<glm::mat4>& instances, u32& vao);
        void init_instancing_cube(const std::vector<glm::mat4>& instances, u32& vao);
        void init_data();

        void draw();

        void draw_debug();
        void draw_debug_line(const glm::vec3& _point1, const glm::vec3& _point2, const glm::vec3& _color);
        void draw_debug_line(const glm::vec3& _point1, const glm::vec3& _point2, const glm::vec3& _color1, const glm::vec3& _color2);
        void draw_debug_cube(const glm::vec3& _center, const f32 _width, const f32 _height, const glm::vec3& _color);
        void draw_debug_sphere(const glm::vec3& _position, const f32 _scale = 1.0f, const glm::quat& orientation = glm::quat());
        void draw_coord_view(const glm::vec3& _position);

        void draw_billboard();
        void draw_line();
        void draw_quad(const core::plane& _p, const glm::vec3 _position, const glm::vec3& _color);
        void draw_cube(const glm::vec3& position, const glm::quat& orientation, const glm::vec3& scale);
        void draw_sphere(const glm::vec3& position, const glm::quat& orientation, const glm::vec3& scale);
        void draw_instancing(const u32 vao, const u32 amount, Texture const * const texture, const glm::vec3& view_position);
        void draw_instancing_cube(const u32 vao, const u32 amount, Texture const * const texture, const glm::vec3& view_position);
        void draw_mesh();

        void resize(u32 _width, u32 _height);
        void set_perspective_projection_matrix(const glm::mat4& _projection);
        void set_orthogonal_projection_matrix(const glm::mat4& _projection);
        void set_view_matrix(const glm::vec3& _eye, float _pitch, float _yaw);
        void set_view_matrix(const glm::vec3& _eye, const glm::vec3& _center, const glm::vec3& _up);
        void set_view_matrix(const glm::mat4& _matrix);

        u32 load_primitive(glm::vec3* vertices, u32 vertices_count, u32* vertices_indices, u32 vertices_indices_count, glm::vec3* normals, u32 normals_count, u32* normals_indices, u32 normals_indices_count);
        void draw_primitive(const u32 vao, const u32 nindices, const Material& material, const glm::vec3& position, const glm::quat& _orientation, const glm::vec3& scale);

        u32 load_scalar_field(core::scalar_field_mesh* mesh);
        void draw_scalar_field(u32 vao, glm::mat4 transform, glm::vec3 view_position, Texture const * const texture, u32 triangle_count);

        void init_transvoxel(voxel::vox_block* block) const;
        void draw_transvoxel(const u32& vao, const u32 indices_count, const glm::vec3& view_position, f32 max_height);

        void load_mesh(Mesh* mesh);
        void draw_mesh(Mesh* mesh, const Material& material, const glm::vec3& position, const glm::quat& orientation, const glm::vec3& scale);
        void release_mesh(Mesh* mesh);

        u32 load_texture(const char* path, bool gamma);

        static constexpr u32 debug_vertices_max_count = 8192;

        u32      m_debug_vertex_count;
        Shader debug_shader;
        glm::vec3 m_debug_vertices[debug_vertices_max_count];
        glm::vec3 m_debug_colors[debug_vertices_max_count];
        u32      m_debug_vao;
        u32      m_debug_vbo;
        u32      m_debug_cbo;

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

        handle<Shader> const * phong_handle;
        handle<Shader> const * instancing_handle;
        handle<Shader> const * sf_handle;
        handle<Shader> const * transvoxel_handle;

        Shader pbr_shader;

        struct text
        {
            std::string value;
            f32 x;
            f32 y;
            f32 scale;
            glm::vec4 color;
        };
        std::vector<text> texts;
        Shader text_shader;
        std::unordered_map<GLchar, RChar> text_characters;
        GLuint text_vao;
        GLuint text_vbo;
        void init_text_2d();
        void draw_text_2d();
        void draw_text_2d(const std::string& _text, const f32 _x, const f32 _y, const f32 _scale, const glm::vec4& _color);

		static constexpr u32 ui_max_vertex_count = 8192;
		u32 ui_vertex_count;
		u32 ui_vao;
		u32 ui_vbo;
		u32 ui_cbo;
		glm::vec3 ui_vertices[ui_max_vertex_count];
		glm::vec3 ui_colors[ui_max_vertex_count];
		Shader ui_shader;
		void init_ui();
		void draw_ui();
		void draw_ui_triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec4& a_color);
		void draw_ui_triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec4& a_color, const glm::vec4& b_color);
		void draw_ui_triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec4& a_color, const glm::vec4& b_color, const glm::vec4& c_color);
        void draw_ui_quad(const f32 x_bottom_left, const f32 y_bottom_left, const f32& width, const f32& height, const glm::vec4& color);
    };
}

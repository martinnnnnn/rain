#pragma once

#include <Windows.h>

#include "GL/glew.h"
#include "gl/GL.h"
#include "gl/GLU.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "core/types.h"
#include "core/config.h"
#include "ogl_shader.h"
#include "win32/win32_application.h"


namespace rain
{
    struct StaticMesh
    {
        GLuint vao;
    };

    struct Camera;

    class Renderer
    {
    public:
        void init();

        void init_default_shaders();

        void init_coord_view();
        void init_quad();
	    void init_cube();
	    void init_sphere();

        void render_coord_view(const glm::vec3& _position);
        void render_quad();
        void render_cube(const glm::vec3& _position, const glm::quat& orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)));
        void render_sphere(const glm::vec3& _position, const glm::quat& orientation = glm::quat(glm::vec3(0.0f,0.0f,0.0f)));

        void clear();

        void set_projection_matrix(const glm::mat4& _projection);
        void set_view_matrix(const glm::vec3& _eye, float _pitch, float _yaw);
        void set_view_matrix(const glm::vec3& _eye, const glm::vec3& _center, const glm::vec3& _up);
        void set_view_matrix(const glm::mat4& _matrix);

        void update_camera();

        Camera* camera;
        GLuint coordviewVAO;
	    u32 coord_index_count;
	    GLuint quadVAO;
	    u32 quad_index_count;
	    GLuint cubeVAO;
	    u32 cube_index_count;
	    GLuint sphereVAO;
	    u32 sphere_index_count;

        //GLuint phong_shader_id;
        //GLuint pbr_shader_id;
        //GLuint coordview_shader_id;
        glm::mat4 proj_map;
        glm::mat4 view_mat;

        //DataIndexer* shaders_indexer_path;
        Shader default_phong;
        Shader default_pbr;
        Shader default_coord_view;
    };
}

#define RAIN_RENDERER rain::Application::get().renderer
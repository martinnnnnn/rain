#pragma once

#include <Windows.h>

#include "GL/glew.h"
#include "gl/GL.h"
#include "gl/GLU.h"
#include "glm/glm.hpp"

#include "core/types.h"

class Shader;

struct StaticMesh
{
    GLuint vao;
};

class Renderer
{
public:
    void init();

    void init_coord_view();
    void render_coord_view(const glm::vec3& _position);

    void init_quad();
    void render_quad();
    
	void init_cube();
    void render_cube(const glm::vec3& _position, const glm::quat& orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)));

	void init_sphere();
    void render_sphere(const glm::vec3& _position, const glm::quat& orientation = glm::quat(glm::vec3(0.0f,0.0f,0.0f)));

    void clear();
    void set_projection_matrix(const glm::mat4& _projection);
    void set_view_matrix(const glm::vec3& _eye, float _pitch, float _yaw);
    void set_view_matrix(const glm::vec3& _eye, const glm::vec3& _center, const glm::vec3& _up);
    void set_view_matrix(const glm::mat4& _matrix);
    void draw_triangle();

    GLuint coordviewVAO;
	u32 coord_index_count;
	GLuint quadVAO;
	u32 quad_index_count;
	GLuint cubeVAO;
	u32 cube_index_count;
	GLuint sphereVAO;
	u32 sphere_index_count;

    unsigned int phong_shader_id;
    unsigned int pbr_shader_id;
    unsigned int coordview_shader_id;
    glm::mat4 projection;
    glm::mat4 view_mat;
};
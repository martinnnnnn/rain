#pragma once

#include <Windows.h>

#include "GL/glew.h"
#include "gl/GL.h"
#include "gl/GLU.h"
#include "glm/glm.hpp"

class Shader;

struct StaticMesh
{
    GLuint vao;
};

class Renderer
{
public:
    void initialize();

    void initialize_coord_view();
    void initialize_quad();
    void initialize_cube();

    void clear();
    void set_projection_matrix(const glm::mat4& _projection);
    void set_view_matrix(const glm::vec3& _eye, float _pitch, float _yaw);
    void render_quad();
    void render_coord_view(const glm::vec3& _position);
    void render_cube(const glm::vec3& _position);
    void draw_triangle();

    GLuint coordviewVAO;
    GLuint quadVAO;
    GLuint cubeVAO;
    unsigned int cubeShaderProgram;
    unsigned int coordviewShaderProgram;
    glm::mat4 projection;
    glm::mat4 view_mat;
};